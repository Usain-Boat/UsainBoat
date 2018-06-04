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

  next_state = S_WAIT_FOR_RADIO;
}

void UsainBoat::state_wait_for_radio(event_e event)
{
  UsainLED::set_pattern(UsainLED::STANDBY);

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
  follow_thread.start(callback(this, &UsainBoat::follow_handler));

  state_event.clear();
}

void UsainBoat::state_relay(event_e event)
{
  relay_thread.start(callback(this, &UsainBoat::relay_handler));

  status_led.set_color(UsainLED::COLOR_YELLOW);

  if (boat_id == BOAT1)
  {
    state_thread.start(relay_handler);
  } else
  {
    state_thread.start(follow_handler);
  }

  state_event.clear();
}

void UsainBoat::relay_handler()
{
  control.set_mode(control.MODE_RC);
}

void UsainBoat::follow_handler()
{
  bool exit = false;
  float angle;
  float distance;
  float standard_distance;

  pid pid_angle = pid(0.1, 100, -100, 0.1, 0.01, 0.5);
  pid pid_distance = pid(0.1, 100, -100, 0.1, 0.01, 0.5);

  control.set_mode(UsainControl::MODE_UC);

  UsainLED::set_color(UsainLED::COLOR_GREEN);

  while (!exit)
  {
    standard_distance = 20;

    if (gps.calculate_distance(longitude, lattitude, &angle, % distance) < 0 )
    {

    }

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

    exit = true;
  }
}

// driver callbacks
void UsainBoat::on_collision_handler()
{
  state_event.set(E_COLLISION);
}

void UsainBoat::on_message_received_handler(const UsainNetworkMessage &message, UsainNetwork *network)
{
  if(message.get_destination() != boat_id)
  {
    // message was not meant for this boat
    return;
  }

  switch (message.get_type())
  {
    case UsainNetworkMessage::GET:

      break;

    case UsainNetworkMessage::POST:

      break;

    case UsainNetworkMessage::BCST:

      break;

    case UsainNetworkMessage::RESP:

      break;

    case UsainNetworkMessage::ERR:

      break;
  }
}

