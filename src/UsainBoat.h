#ifndef USAINBOAT_H
#define USAINBOAT_H

#include "mbed.h"

#include "usain_gps.h"
#include "usain_imu.h"
#include "usain_network.h"
#include "usain_control.h"
#include "usain_led.h"
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
  E_COLLISION, E_NEW_GPS_DATA
} event_e;

struct other_coordinates{
  double latitude;
  double longitude;
};


class UsainBoat
{
 public:
  void start();

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
  void on_new_gps_data_handler();

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
  osThreadId thread_id;

  other_coordinates coor_other_boat;

};

#endif
