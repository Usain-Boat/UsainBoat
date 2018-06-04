#ifndef USAINBOAT_H
#define USAINBOAT_H

#include "mbed.h"

// TODO: add files to project so it can compile
#include "../../gps/src/usain_gps.h"
#include "../../imu/src/usain_imu.h"
#include "../../radio/src/usain_network.h"
#include "../../rc_control/src/usain_control.h"
#include "../../rc_control/src/usain_led.h"
#include "pid.h"

typedef enum
{
  NOERR
} error_e;

typedef enum
{
  HOST,
  BOAT1,
  BOAT2
} boat_id_t;

typedef enum
{
  S_NO, S_INIT, S_WAIT_FOR_RADIO,
  S_FOLLOW, S_MANUAL, S_RELAY
} state_e;

typedef enum
{
  E_NO, E_ERROR, E_READY, E_INIT, E_START_FOLLOW,
  E_START_MANUAL, E_START_RELAY, E_WAIT_FOR_NEXT_MESSAGE,
  E_COLLISION
} event_e;

class UsainBoat
{
 public:
  void run();

 private:
  // state handlers
  void state_init();

  void state_wait_for_radio(event_e event);

  void state_manual(event_e event);

  void state_follow(event_e event);

  void state_relay(event_e event);

  void relay_handler();

  void follow_handler();

  // driver callbacks
  void on_collision_handler();
  void on_message_received_handler(const UsainNetworkMessage &message, UsainNetwork *network);

  boat_id_t boat_id;

  EventFlags state_event;
  state_e current_state;
  state_e next_state;

  UsainNetwork network;
  UsainControl control;
  UsainGPS gps;
  UsainIMU imu;
  UsainLED status_led;

  Thread state_thread;
  Thread follow_thread;
  Thread relay_thread;
};

#endif
