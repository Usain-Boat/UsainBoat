//
// Created by Jamie on 25-4-2018.
//

#include "usain_control.h"

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

UsainControl::UsainControl() : _motor_throttle_in(RECEIVER_THROTTLE_PIN),
                               _motor_steer_in(RECEIVER_STEER_PIN),
                               _motor_left_out(MOTOR_LEFT_OUT_PIN),
                               _motor_right_out(MOTOR_RIGHT_OUT_PIN)
{

}

void UsainControl::set_mode(UsainControl::mode_t mode)
{
  if (mode == MODE_RC)
  {
    _rc_handler.detach();
    _rc_handler.attach_us(callback(this, &UsainControl::handle_rc), 1000);

    _motor_left_out.write(0.0f);
    _motor_right_out.write(0.0f);

  } else
  {
    _rc_handler.detach();
  }

  _mode = mode;
}

UsainControl::mode_t UsainControl::get_mode() const
{
  return _mode;
}

void UsainControl::set_motor(UsainControl::motor_t motor, float duty_cycle)
{
  if (_mode == MODE_RC)
    return;

  if (duty_cycle > 1.0)
  {
    duty_cycle = 1.0;
  }

  // convert set range for motor (1ms - 2ms)
  duty_cycle = 1000 + (duty_cycle * 1000);

  switch (motor)
  {
    case MOTOR_RIGHT:_motor_right_out.pulsewidth_us(duty_cycle);
      break;

    case MOTOR_LEFT:_motor_left_out.pulsewidth_us(duty_cycle);
      break;
  }
}

#define PWM_MAX   0.097F
#define PWM_MIN   0.044F

void UsainControl::handle_rc()
{
  // prevent the motors to run while there is no connection
  if (_motor_throttle_in.pulsewidth() > 0.0025)
  {
    _motor_left_out.write(0.0);
    _motor_right_out.write(0.0);
    return;
  }

  float throttle_MOTOR = _motor_throttle_in.dutycycle();
  float throttle_dc = ((_motor_throttle_in.dutycycle() - PWM_MIN) / (PWM_MAX - PWM_MIN));
  float steer_dc = ((_motor_steer_in.dutycycle() - PWM_MIN) / (PWM_MAX - PWM_MIN));

  if (steer_dc < (PWM_MAX - PWM_MIN) / 2 + PWM_MIN)
  {
    _motor_left_out.write(throttle_MOTOR);
    _motor_right_out.write((PWM_MAX - PWM_MIN) * (steer_dc * 2) * throttle_dc + PWM_MIN);
  } else if (steer_dc > (PWM_MAX - PWM_MIN) / 2 + PWM_MIN)
  {
    _motor_left_out.write((PWM_MAX - PWM_MIN) * ((1 - steer_dc) * 2) * throttle_dc + PWM_MIN);
    _motor_right_out.write(throttle_MOTOR);
  } else
  {
    _motor_left_out.write(throttle_MOTOR);
    _motor_right_out.write(throttle_MOTOR);
  }
}

/////////////////////////////////////
/// PWM Input implementation
/////////////////////////////////////

PwmIn::PwmIn(PinName p) : _p(p)
{
  _p.rise(callback(this, &PwmIn::rise));
  _p.fall(callback(this, &PwmIn::fall));
  _period = 0.0;
  _pulsewidth = 0.0;
  _t.start();
}

float PwmIn::period()
{
  return _period;
}

float PwmIn::pulsewidth()
{
  return _pulsewidth;
}

float PwmIn::dutycycle()
{
  return _pulsewidth / _period;
}

void PwmIn::rise()
{
  _period = _t.read();
  _t.reset();
}

void PwmIn::fall()
{
  _pulsewidth = _t.read();
}
