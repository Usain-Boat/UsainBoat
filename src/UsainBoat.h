#ifndef USAINBOAT_H
#define USAINBOAT_H

typedef int error_t;

typedef enum {NOERR} error_e;

typedef enum {S_NO, S_START, S_INIT, S_WAIT_FOR_RADIO_MESSAGE,
  S_FOLLOW, S_MANUAL, S_RELAY}
    state_e;
typedef enum {E_NO, E_ERROR, E_READY, E_INIT, E_START_FOLLOW,
  E_START_MANUAL, E_START_RELAY, E_WAIT_FOR_NEXT_MESSAGE}
    event_e;

extern event_e event;

void statemachine(event_e event);
event_e initialize(void);



#endif
