#include "mbed.h"
#include "UsainBoat.cpp"


int main()
{

  event_e event = E_INIT;
  statemachine(event);

  return 0;
}
