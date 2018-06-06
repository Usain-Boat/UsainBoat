#include <string>
#include <vector>
#include "UsainBoat.h"

void UsainBoat::start()
{
  // trigger initialize state. only after startup
  state_event.set(E_NO);

  while (true)
  {
    // not clearing the event flag will result in a loop in the statemachine
    event_e event = (event_e) state_event.wait_all(0, static_cast<uint32_t>(-1), false);

    switch (current_state)
    {
      case S_INIT:
        state_init();
        break;

      case S_WAIT_FOR_RADIO:
        state_wait_for_radio(event);
        break;

      case S_FOLLOW:
        state_follow(event);
        break;

      case S_MANUAL:
        state_manual(event);
        break;

      case S_RELAY:
        state_relay(event);
        break;

      default:
        printf("Unknown state");
        break;
    }

    current_state = next_state;
  }
}

void UsainBoat::state_init()
{
  UsainLED::set_pattern(UsainLED::LOADING);

  if (imu.init() < 0)
  {
    error("imu init failure\n");
  }

  if (network.init() < 0)
  {
    error("usain network init failure\n");
  }

  if (gps.init() < 0)
  {
    error("usain gps init failure\n");
  }

  DigitalIn determine_boat(PA_4);
  boat_id = determine_boat ? BOAT1 : BOAT2;

  // register handlers
  imu.register_on_collision(callback(this, &UsainBoat::on_collision_handler));
  network.register_message_received(callback(this, &UsainBoat::on_message_received_handler));
  gps.on_new_message(callback(this, &UsainBoat::on_new_gps_data_handler));

  next_state = S_WAIT_FOR_RADIO;
}

void UsainBoat::state_wait_for_radio(event_e event)
{
  UsainLED::set_pattern(UsainLED::STANDBY);

  control.set_mode(UsainControl::MODE_UC);
  control.set_motor(UsainControl::MOTOR_RIGHT, 0);
  control.set_motor(UsainControl::MOTOR_LEFT, 0);

  state_event.clear();
}

void UsainBoat::state_manual(event_e event)
{
  UsainLED::set_pattern(UsainLED::INTERACTIVE);
  control.set_mode(UsainControl::MODE_RC);
  state_event.clear();
}

void UsainBoat::state_follow(event_e event)
{
  switch(event){
    case E_START_FOLLOW :
      follow_thread.start(callback(this, &UsainBoat::follow_handler));
      break;

    case E_WAIT_FOR_NEXT_MESSAGE :
      follow_thread.signal_set(E_WAIT_FOR_NEXT_MESSAGE);
      next_state = S_WAIT_FOR_RADIO;
      break;

    default:
      // do nothing
      break;

  }

  state_event.clear();
}

void UsainBoat::state_relay(event_e event)
{
  switch(event)
  {
    case E_START_RELAY:
      relay_thread.start(callback(this, &UsainBoat::relay_handler));
      break;

    case E_COLLISION:
      relay_thread.signal_set(E_COLLISION);
      osSignalSet(thread_id, E_COLLISION);
      break;

    case E_WAIT_FOR_NEXT_MESSAGE:
      osSignalSet(thread_id, E_WAIT_FOR_NEXT_MESSAGE);
      next_state = S_WAIT_FOR_RADIO;
      break;

    default:
      // do nothing
      break;

  }

  state_event.clear();
}

void UsainBoat::relay_handler()
{
  bool move = false;
  bool exit = false;

  if(boat_id == BOAT1){
    move = true;
  }

  while(!exit){

    if(move){
      control.set_mode(control.MODE_RC);

    }else{
      control.set_mode(control.MODE_UC);
      control.set_motor(control.MOTOR_LEFT, 0);
      control.set_motor(control.MOTOR_RIGHT, 0);
    }

    osEvent v = relay_thread.signal_wait(0, osWaitForever);

    if(v.value.signals == E_COLLISION){
      move = !move;
    }
    if(v.value.signals == E_WAIT_FOR_NEXT_MESSAGE){
      exit = true;
    }
  }
}

void UsainBoat::follow_handler()
{
  bool exit = false;
  double angle = 0;
  double distance = 0;

  UsainLED::set_color(UsainLED::COLOR_GREEN);



  if (boat_id == BOAT1){
    control.set_mode(UsainControl::MODE_RC);
  }else
  {

    pid pid_angle = pid(0.1, 100, -100, 0.1, 0.01, 0.5);
    pid pid_distance = pid(0.1, 100, -100, 0.1, 0.01, 0.5);

    control.set_mode(UsainControl::MODE_UC);

    while (!exit)
    {
      if (angle <= 0)
      {
        control.set_motor(control.MOTOR_RIGHT,
                          static_cast<float>(0.2 * pid_distance.calculate(0.8, distance)
                              * pid_angle.calculate(0, angle)));
        control.set_motor(control.MOTOR_LEFT,
                          static_cast<float>(0.2 * pid_distance.calculate(0.8, distance)
                              / pid_angle.calculate(0, angle)));
      } else
      {
        control.set_motor(control.MOTOR_RIGHT,
                          static_cast<float>(0.2 * pid_distance.calculate(0.8, distance)
                              / pid_angle.calculate(0, angle)));
        control.set_motor(control.MOTOR_LEFT,
                          static_cast<float>(0.2 * pid_distance.calculate(0.8, distance)
                              * pid_angle.calculate(0, angle)));
      }

      osEvent v = follow_thread.signal_wait(0, 5);

      if (v.value.signals == E_NEW_GPS_DATA)
      {
        gps.calculate_distance(coor_other_boat.latitude, coor_other_boat.longitude, &distance, &angle);
      }

      if (v.value.signals == E_WAIT_FOR_NEXT_MESSAGE)
      {
        exit = true;
      }
    }
  }
}

// driver callbacks
void UsainBoat::on_collision_handler()
{
  state_event.set(E_COLLISION);
}

void UsainBoat::on_new_gps_data_handler()
{
  state_event.set(E_NEW_GPS_DATA);
}

void UsainBoat::on_message_received_handler(const UsainNetworkMessage &message, UsainNetwork *network)
{
  if (message.get_destination() != boat_id)
  {
    // message was not meant for this boat
    return;
  }

  const uint8_t *data = message.get_data();
  string data_string;

  switch (message.get_type())
  {
    case UsainNetworkMessage::GET:

      break;

    case UsainNetworkMessage::POST:

      break;

    case UsainNetworkMessage::BCST:

      break;

    case UsainNetworkMessage::RESP:

      for (int i = 0; i < 246; i++)
      {
        data_string.push_back(data[i]);
      }
      if (data_string.find("gps")){
        if (coor_other_boat.longitude == 1.0 || coor_other_boat.latitude == 1.0)
        {
          coor_other_boat.longitude = 1.0;
          coor_other_boat.latitude = 1.0;
          state_event.set(E_NEW_GPS_DATA);
        }
      }
      break;

    case UsainNetworkMessage::ERR:

      break;
  }
}





