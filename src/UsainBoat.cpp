#include <usain_network_message.h>
#include <cmsis_os.h>
#include <arm_math.h>
#include <drv_gps.h>
#include "UsainBoat.h"

static UsainLED status_led;

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

UsainBoat::UsainBoat() : determine_boat(A1),
                         motor_left_monitor(A4, 0.1),
                         motor_right_monitor(A3, 0.1)
{
  imu = new UsainIMU();
  network = new UsainNetwork();
  gps = new UsainGPS();
  control = new UsainControl();
}

void UsainBoat::start()
{
  // trigger initialize state. only after startup
  state_event.set(E_NO);
  current_state = S_INIT;

  while (true)
  {
    // not clearing the event flag will result in a loop in the statemachine
    event_e event = (event_e) state_event.wait_all(0, static_cast<uint32_t>(-1), false);

    switch (current_state)
    {
      case S_INIT:
        printf("== state:S_INIT event:%d\n", (int) event);
        state_init();
        break;

      case S_WAIT_FOR_RADIO:
        printf("== state:S_WAIT_FOR_RADIO event:%d\n", (int) event);
        state_wait_for_radio(event);
        break;

      case S_FOLLOW:
        printf("== state:S_FOLLOW event:%d\n", (int) event);
        state_follow(event);
        break;

      case S_MANUAL:
        printf("== state:S_MANUAL event:%d\n", (int) event);
        state_manual(event);
        break;

      case S_RELAY:
        printf("== state:S_RELAY event:%d\n", (int) event);
        state_relay(event);
        break;

      default:
        printf("Unknown state");
        state_event.clear();
        break;
    }

    current_state = next_state;
  }
}

void UsainBoat::state_init()
{
  UsainLED::set_pattern(UsainLED::LOADING);

  if (imu->init() < 0)
  {
    error("imu init failure\n");
  }

  imu->enable();

  if (!network->init())
  {
    error("network init failure\n");
  }

  if (gps->init() != 0)
  {
    error("gps init failure\n");
  }

  boat_id = determine_boat == 0 ? BOAT1 : BOAT2;

  printf("== boat ID: %d\n", boat_id);

  // register handlers
  imu->register_on_collision(callback(this, &UsainBoat::on_collision_handler));
  network->register_message_received(callback(this, &UsainBoat::on_message_received_handler));
  gps->on_new_message(callback(this, &UsainBoat::on_new_gps_data_handler));

  current_mode = 0;

  next_state = S_WAIT_FOR_RADIO;
  UsainLED::set_pattern(UsainLED::STANDBY);

  control->set_mode(UsainControl::MODE_UC);
  control->set_motor(UsainControl::MOTOR_RIGHT, 0);
  control->set_motor(UsainControl::MOTOR_LEFT, 0);
}

void UsainBoat::state_wait_for_radio(event_e event)
{
  switch (event)
  {
    case E_START_MANUAL:
      UsainLED::set_pattern(UsainLED::INTERACTIVE);
      control->set_mode(UsainControl::MODE_RC);
      break;

    case E_START_FOLLOW:
      follow_thread = new Thread(callback(this, &UsainBoat::follow_handler));
      break;

    case E_START_RELAY:
      relay_thread = new Thread(callback(this, &UsainBoat::relay_handler));
      break;

    default:
      break;
  }

  state_event.clear();
}

void UsainBoat::state_manual(event_e event)
{
  switch (event)
  {
    case E_WAIT_FOR_NEXT_MESSAGE:
      UsainLED::set_pattern(UsainLED::STANDBY);
      control->set_mode(UsainControl::MODE_UC);
      control->set_motor(UsainControl::MOTOR_RIGHT, 0);
      control->set_motor(UsainControl::MOTOR_LEFT, 0);
      break;

    case E_START_FOLLOW:
      follow_thread = new Thread(callback(this, &UsainBoat::follow_handler));
      break;

    case E_START_RELAY:
      relay_thread = new Thread(callback(this, &UsainBoat::relay_handler));
      break;

    default:
      break;

  }
  state_event.clear();
}

void UsainBoat::state_follow(event_e event)
{
  switch (event)
  {
    case E_WAIT_FOR_NEXT_MESSAGE :
      follow_thread->signal_set(E_WAIT_FOR_NEXT_MESSAGE);
      control->set_mode(UsainControl::MODE_UC);
      control->set_motor(UsainControl::MOTOR_RIGHT, 0);
      control->set_motor(UsainControl::MOTOR_LEFT, 0);
      break;

    case E_START_MANUAL:
      follow_thread->signal_set(E_START_MANUAL);
      UsainLED::set_pattern(UsainLED::INTERACTIVE);
      control->set_mode(UsainControl::MODE_RC);
      break;

    case E_START_RELAY:
      follow_thread->signal_set(E_START_RELAY);
      relay_thread->start(callback(this, &UsainBoat::relay_handler));
      break;

    case E_NEW_GPS_DATA:
      follow_thread->signal_set(E_NEW_GPS_DATA);
      break;

    default:
      break;
  }

  state_event.clear();
}

void UsainBoat::state_relay(event_e event)
{
  switch (event)
  {
    case E_WAIT_FOR_NEXT_MESSAGE:
      relay_thread->signal_set(E_WAIT_FOR_NEXT_MESSAGE);
      control->set_mode(UsainControl::MODE_UC);
      control->set_motor(UsainControl::MOTOR_RIGHT, 0);
      control->set_motor(UsainControl::MOTOR_LEFT, 0);
      break;

    case E_START_MANUAL:
      relay_thread->signal_set(E_START_MANUAL);
      UsainLED::set_pattern(UsainLED::INTERACTIVE);
      control->set_mode(UsainControl::MODE_RC);
      break;

    case E_START_FOLLOW:
      relay_thread->signal_set(E_START_FOLLOW);
      follow_thread->start(callback(this, &UsainBoat::follow_handler));
      break;

    case E_COLLISION:
      relay_thread->signal_set(E_COLLISION);
      break;

    case E_CONTROLLER_COLLISION:
      relay_thread->signal_set(E_CONTROLLER_COLLISION);
      break;

    default:
      break;

  }

  state_event.clear();
}

void UsainBoat::relay_handler()
{
  printf("== relay started\n");

//  bool move = false;
  bool exit = false;

//  if (boat_id == BOAT1)
//  {
//    move = true;
//  } else
//  {
//    imu->enable();
//  }

  if (boat_id == BOAT1)
  {
    control->set_mode(control->MODE_RC);
    imu->disable();
  } else
  {
    control->set_mode(control->MODE_UC);
    control->set_motor(control->MOTOR_LEFT, 0);
    control->set_motor(control->MOTOR_RIGHT, 0);
    imu->enable();
  }

  while (!exit)
  {
    osEvent v = relay_thread->signal_wait(0, osWaitForever);

    if (v.value.signals == E_COLLISION)
    {
      imu->disable();

      UsainNetworkMessage m;

      m.set_type(UsainNetworkMessage::BCST);
      m.set_source(boat_id);
      m.set_destination(boat_id == BOAT1 ? BOAT2 : BOAT1);
      m.add_parameter("collision");

      network->send(m);

      control->set_mode(control->MODE_RC);
    }

    if (v.value.signals == E_WAIT_FOR_NEXT_MESSAGE ||
        v.value.signals == E_START_MANUAL||
        v.value.signals == E_START_FOLLOW ||
        v.value.signals == E_CONTROLLER_COLLISION)
    {
      exit = true;
      control->set_mode(control->MODE_UC);
      control->set_motor(control->MOTOR_LEFT, 0);
      control->set_motor(control->MOTOR_RIGHT, 0);
      imu->disable();
    }
  }

  delete(relay_thread);
}

void UsainBoat::follow_handler()
{
  bool exit = false;
  double angle = 0;
  double distance = 0;

  UsainLED::set_color(UsainLED::COLOR_GREEN);

  arm_pid_instance_f32 PID_accelerate = {};
  PID_accelerate.Kp = 1.0;
  PID_accelerate.Ki = 1.0;
  PID_accelerate.Kd = 0.0;
  arm_pid_init_f32(&PID_accelerate, 1);

  arm_pid_instance_f32 PID_bearing = {};
  PID_bearing.Kp = 1.0;
  PID_bearing.Ki = 1.0;
  PID_bearing.Kd = 0.0;
  arm_pid_init_f32(&PID_bearing, 1);

  float accelerate = 0;
  float steer = 0;
  float distance_to_dest_PID, bearing_PID;

  double longitude_dest, latitude_dest, distance_to_dest, home_to_dest_bearing, compass_to_dest_bearing;

  control->set_mode(UsainControl::MODE_UC);
  while (!exit)
  {
    osEvent v = follow_thread->signal_wait(0, 3000);

    //!!!!!!!!!!!!!!!!!!!!!!!!
    // INSERT: GET GPS DATA FROM OTHER BOAT
    //!!!!!!!!!!!!!!!!!!!!!!!!
    gps->calculate_distance(latitude_dest, longitude_dest, &distance_to_dest, &home_to_dest_bearing);
    compass_to_dest_bearing = imu->get_compass() - home_to_dest_bearing;

    if (1) // accelerate
    {
      distance_to_dest_PID = arm_pid_f32(&PID_accelerate, 1000 - distance_to_dest);
      //Range limit the output
      if (distance_to_dest_PID < -2000.0)
      {
        distance_to_dest_PID = -2000.0f;
      } else if (distance_to_dest_PID > 9000.0)
      {
        distance_to_dest_PID = 9000.0;
      }
    }
    if (1) // steering
    {
      bearing_PID = arm_pid_f32(&PID_bearing, compass_to_dest_bearing);
      //Range limit the output
      if (bearing_PID < -160.0)
      {
        bearing_PID = -160.0f;
      } else if (bearing_PID > 160.0)
      {
        bearing_PID = 160.0;
      }
    }
    if (distance_to_dest_PID > 1000.0)
    {

      accelerate = fmap(distance_to_dest_PID, 0.0, 9000.0, 0.0, 0.6);
      steer = fmap(fabs(bearing_PID), 0.0, 160.0, 0.0, 1.0);

      if (bearing_PID < 0) //go left
      {
        control->set_motor(UsainControl::MOTOR_LEFT, steer * accelerate);
        control->set_motor(UsainControl::MOTOR_RIGHT, 0.6);
      } else //go right
      {
        control->set_motor(UsainControl::MOTOR_LEFT, 0.6);
        control->set_motor(UsainControl::MOTOR_RIGHT, steer * accelerate);
      }
    }

    if (v.value.signals == E_WAIT_FOR_NEXT_MESSAGE || v.value.signals == E_START_MANUAL
        || v.value.signals == E_START_RELAY)
    {
      exit = true;
    }
  }

  delete(follow_thread);
}

// driver callbacks
void UsainBoat::on_collision_handler()
{
  state_event.set(E_COLLISION);
}

void UsainBoat::on_new_gps_data_handler(AdafruitUltimateGPS::gprmc_data_t data)
{
  if(data.validity[0] == 'A'){
    gps->get_average_gps(&data.latitude_fixed, &data.longitude_fixed);

    UsainNetworkMessage m;

    m.set_type(UsainNetworkMessage::BCST);
    m.set_source(boat_id);
    m.set_destination(boat_id == BOAT1 ? BOAT2 : BOAT1);
    m.add_parameter("latitude", static_cast<float>(data.latitude_fixed));
    m.add_parameter("longitude", static_cast<float>(data.longitude_fixed));

    network->send(m);
  }
}

void UsainBoat::on_message_received_handler(const UsainNetworkMessage &message, UsainNetwork *network)
{
  if (message.get_destination() != boat_id)
  {
    // message was not meant for this boat
    return;
  }

  UsainNetworkMessage reply;
  UsainNetworkMessage::paramater_t params[16];
  int paramc = message.get_paramaters(params, 16);

  const uint8_t *data = message.get_data();

  switch (message.get_type())
  {
    case UsainNetworkMessage::GET:
    {
      reply.set_type(UsainNetworkMessage::RESP);
      reply.set_source(boat_id);
      reply.set_destination(message.get_source());

      for (int i = 0; i < paramc; i++)
      {
        if (strcmp(params[i].name, "current1") == 0)
        {

          reply.add_parameter("current1", motor_left_monitor.get_current());
        } else if (strcmp(params[i].name, "current2") == 0)
        {
          reply.add_parameter("current2", motor_right_monitor.get_current());
        } else if (strcmp(params[i].name, "longitude") == 0)
        {
          reply.add_parameter("longitude", coor_boat.longitude);
        } else if (strcmp(params[i].name, "latitude") == 0)
        {
          reply.add_parameter("latitude", coor_boat.latitude);
        } else if (strcmp(params[i].name, "heading") == 0)
        {
          reply.add_parameter("heading", imu->get_compass());
        } else if (strcmp(params[i].name, "mode") == 0)
        {
          reply.add_parameter("mode", current_mode);
        }
      }

//      printf("replying: %s\n", reply.get_data());

      network->send(reply);

      break;
    }

    case UsainNetworkMessage::POST:
    {
      //check if wallstation has send the post
      if (message.get_source() == 0)
      {
        printf("parameter : %s\n", params[0].name);
        for (int i = 0; i < paramc; i++)
        {
          if (strcmp(params[i].name, "mode") == 0)
          {
            switch (params[i].value[0])
            {
              case '0':
                current_mode = 0;
                next_state = S_WAIT_FOR_RADIO;
                state_event.set(E_WAIT_FOR_NEXT_MESSAGE);
                break;

              case '1':
                current_mode = 1;
                next_state = S_MANUAL;
                state_event.set(E_START_MANUAL);
                break;

              case '2':
                current_mode = 2;
                next_state = S_FOLLOW;
                state_event.set(E_START_FOLLOW);
                break;

              case '3':
                current_mode = 3;
                next_state = S_RELAY;
                state_event.set(E_START_RELAY);
                break;

              default:
                //error wrong mode
                break;
            }
          }
        }
      }
      break;
    }
    case UsainNetworkMessage::BCST:
    {
      for (int i = 0; i < paramc; i++)
      {
        if (strcmp(params[i].name, "collision") == 0)
        {
          state_event.set(E_CONTROLLER_COLLISION);
        } else if (strcmp(params[i].name, "latitude") == 0)
        {
          coor_other_boat.latitude = static_cast<float>(atof(params[i].value));
        } else if (strcmp(params[i].name, "longitude") == 0)
        {
          coor_other_boat.longitude = static_cast<float>(atof(params[i].value));
          state_event.set(E_NEW_GPS_DATA);
        }
      }

//      uint8_t comma = 0;
//      char latitude[9];
//
//      for (int i = 4; i < message.get_data_size() - 1; i++)
//      {
//        if (data[i] == ',')
//        {
//          comma = i + 1;
//          break;
//        }
//        latitude[i] = data[i];
//      }
//
//      char longitude[8];
//
//      for (int i = comma; i < message.get_data_size() - 1; i++)
//      {
//        latitude[i - comma] = data[i];
//      }
//
//      coor_other_boat.latitude = atof(latitude);
//      coor_other_boat.longitude = atof(longitude);
//      state_event.set(E_NEW_GPS_DATA);

      break;
    }
    case UsainNetworkMessage::RESP:

      break;

    case UsainNetworkMessage::ERR:

      break;
  }
}

