#include "UsainBoat.h"
#include "mbed.h"
#include "../../gps/src/usain_gps.cpp"
#include "../../imu/src/usain_imu.cpp"
#include "../../radio/src/usain_network.cpp"
#include "../../rc_control/src/usain_control.cpp"


static state_e currentState = S_START;

Thread thread_gps;
Thread thread_imu;
Thread thread_radio;
Thread thread_state;

EventQueue queue;

void thread_function_gps();
void thread_function_imu();
void thread_function_radio();
void thread_function_wait_for_message();
void thread_function_follow();
void thread_function_manual();
void thread_function_relay();

void statemachine(event_e event) {
  state_e nextState = S_NO;

  switch(currentState) {
    case S_START:
      nextState = S_INIT;
      break;

    case S_INIT:
      event = initialize();
      nextState = S_WAIT_FOR_RADIO_MESSAGE;
      break;

    case S_WAIT_FOR_RADIO_MESSAGE:
      thread_state.start(thread_function_wait_for_message);


      nextState = S_RELAY;
      nextState = S_MANUAL;
      nextState = S_FOLLOW;
      break;

    case S_FOLLOW:
      thread_state.start(thread_function_follow);

      nextState = S_WAIT_FOR_RADIO_MESSAGE;
      break;

    case S_MANUAL:
      thread_state.start(thread_function_manual);
      nextState = S_WAIT_FOR_RADIO_MESSAGE;
      break;

    case S_RELAY:
      thread_state.start(thread_function_relay);
      nextState = S_WAIT_FOR_RADIO_MESSAGE;
      break;

default:
      printf("Unknown state");
      break;
  }
  currentState = nextState;
}

event_e initialize() {
  printf("Start of the statemachine");
  thread_gps.start(thread_function_gps);
  thread_imu.start(thread_function_imu);
  thread_radio.start(thread_function_radio);


  return E_READY;
}


void thread_function_gps(){
  UsainGPS gps;
  gps.init();

  while(1){



  }
}


void thread_function_imu(){
  UsainIMU imu;
  imu.init();

  while(1){

  }
}

void thread_function_radio(){
  UsainNetwork network;
  network.init();

  while(1){}
}

