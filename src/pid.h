//
// Created by Victor van Wijhe on 4-6-2018.
//

#ifndef USAIN_BOAT_PID_H
#define USAIN_BOAT_PID_H


// TODO: add files to project so it can compile
#include "../../gps/src/usain_gps.h"
#include "../../imu/src/usain_imu.h"
#include "../../radio/src/usain_network.h"
#include "../../rc_control/src/usain_control.h"
#include "../../rc_control/src/usain_led.h"


class pid
{
 public:
  // Kp -  proportional gain
  // Ki -  Integral gain
  // Kd -  derivative gain
  // dt -  loop interval time
  // max - maximum value of manipulated variable
  // min - minimum value of manipulated variable
  pid( double dt, double max, double min, double Kp, double Kd, double Ki );

  // Returns the manipulated variable given a setpoint and current process value
  float calculate( float setpoint, float pv );

  ~pid();

 private:
  float _dt;
  float _max;
  float _min;
  float _Kp;
  float _Kd;
  float _Ki;
  float _pre_error;
  float _integral;
};


#endif
