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
      case S_INIT:state_init();
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

  // register handlers
  imu.register_on_collision(callback(this, &UsainBoat::on_collision_handler));
  network.register_message_received(callback(this, &UsainBoat::on_message_received_handler));

  next_state = S_WAIT_FOR_RADIO;
  state_event.clear();
}

void UsainBoat::state_wait_for_radio(event_e event)
{
  status_led.set_pattern(UsainLED::STANDBY);
}

void UsainBoat::state_manual(event_e event)
{

}

void UsainBoat::state_follow(event_e event)
{

}

void UsainBoat::state_relay(event_e event)
{

}

// driver callbacks
void UsainBoat::on_collision_handler()
{
  state_event.set(E_COLLISION);
}
void UsainBoat::on_message_received_handler(UsainNetworkMessage message)
{
//TODO check what the message is, message format is required

  switch ()
  {
    case :

      state_event.set(E_START_FOLLOW);
      break;

    case :

      state_event.set(E_START_MANUAL);
      break;

    case :

      state_event.set(E_START_RELAY);
      break;

  }
}

