//
// Created by Victor van Wijhe on 4-6-2018.
//


#include "pid.h"

pid::pid(double dt, double max, double min, double Kp, double Kd, double Ki)
{
  pid::_dt = dt;
  pid::_max = max;
  pid::_min = min;
  pid::_Kp = Kp;
  pid::_Kd = Kd;
  pid::_Ki = Ki;

}


float pid::calculate(float setpoint, float pv)
{
  // Calculate error
  float error = setpoint - pv;

  // Proportional term
  float Pout = _Kp * error;

  // Integral term
  _integral += error * _dt;
  float Iout = _Ki * _integral;

  // Derivative term
  float derivative = (error - _pre_error) / _dt;
  float Dout = _Kd * derivative;

  // Calculate total output
  float output = Pout + Iout + Dout;

  // Restrict to max/min
  if( output > _max )
    output = _max;
  else if( output < _min )
    output = _min;

  // Save error to previous error
  _pre_error = error;

  return output;
}

