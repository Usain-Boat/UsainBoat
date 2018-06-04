#include "UsainBoat.h"

void UsainBoat::run()
{
  // trigger initialize state. only after startup
  state_event.set(E_NO);

  while (true)
  {
    // not clearing the event flag will result in a loop in the statemachine
    auto event = (event_e) state_event.wait_all(0, static_cast<uint32_t>(-1), false);

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

      default:printf("Unknown state");
        break;
    }

    current_state = next_state;
  }
}

void UsainBoat::state_init()
{
  status_led.set_pattern(UsainLED::LOADING);

  DigitalIn determine_boat(PA_4);

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

  Boat = determine_boat ? Boat1 : Boat2;


  // register handlers
  imu.register_on_collision(callback(this, &UsainBoat::on_collision_handler));
  network.register_message_received(Callback(this, &UsainNetworkMessage::get_data));

  next_state = S_WAIT_FOR_RADIO;
  state_event.clear();
}

void UsainBoat::state_wait_for_radio(event_e event)
{
  status_led.set_pattern(UsainLED::STANDBY);

}

void UsainBoat::state_manual(event_e event)
{
  status_led.set_pattern(UsainLED::);
  control.set_mode(UsainControl::MODE_RC);
}

void UsainBoat::state_follow(event_e event)
{
  bool exit = false;
  float angle;
  float distance;
  float standard_distance;

  PID pid_angle = PID(0.1, 100, -100, 0.1, 0.01, 0.5);
  PID pid_distance =  PID(0.1, 100, -100, 0.1, 0.01, 0.5);

  control.set_mode(UsainControl::MODE_UC);

  status_led.set_color(UsainLED::COLOR_GREEN);

  while(!exit)
  {
    standard_distance = 20;

    if(gps.get_distance_centimeter(longitude, lattitude, &angle, %distance) < 0 )
    {

    }

    if ( angle <= 0)
    {
      control.set_motor(control.MOTOR_RIGHT,
                        static_cast<float>(0.2 * pid_distance.calculate(0.8, distance) * pid_angle.calculate(0, angle)));
      control.set_motor(control.MOTOR_LEFT,
                        static_cast<float>(0.2 * pid_distance.calculate(0.8, distance) / pid_angle.calculate(0, angle)));
    }else
    {
      control.set_motor(control.MOTOR_RIGHT,
                        static_cast<float>(0.2 * pid_distance.calculate(0.8, distance) / pid_angle.calculate(0, angle)));
      control.set_motor(control.MOTOR_LEFT,
                        static_cast<float>(0.2 * pid_distance.calculate(0.8, distance) * pid_angle.calculate(0, angle)));
    }

    exit = true;
    5
  }
}

void UsainBoat::state_relay(event_e event)
{
  status_led.set_color(UsainLED::COLOR_YELLOW);

  if ( Boat ==  Boat1){
    state_thread.start(state_relay_move);
  }else{
    state_thread.start(state_relay_stay);
  }


}

void UsainBoat::state_relay_move()
{
  control.set_mode(control.MODE_RC);

  auto event = (event_e) state_event.wait_all(E_COLLISION, static_cast<uint32_t>(-1), false);

}

void UsainBoat::state_relay_stay()
{

}







// driver callbacks
void UsainBoat::on_collision_handler()
{
  state_event.set(E_COLLISION);
}
void UsainBoat::on_message_received_handler()
{
//TODO check what the message is, message format is required

  switch ()
  {
    case :

      state_event.set(E_START_FOLLOW);
      next_state = S_FOLLOW;
      break;

    case :

      state_event.set(E_START_MANUAL);
      next_state = S_MANUAL;
      break;

    case :

      state_event.set(E_START_RELAY);
      next_state = S_RELAY;
      break;

  }
}

