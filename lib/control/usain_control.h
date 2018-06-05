//
// Created by Jamie on 25-4-2018.
//

#ifndef RC_CONTROL_USAIN_CONTROL_H
#define RC_CONTROL_USAIN_CONTROL_H

#include "mbed.h"

class PwmIn
{
public:
    /** Create a PwmIn
     *
     * @param p The pwm input pin (must support InterruptIn)
     */
    PwmIn(PinName p);

    /** Read the current period
     *
     * @returns the period in seconds
     */
    float period();

    /** Read the current pulsewidth
     *
     * @returns the pulsewidth in seconds
     */
    float pulsewidth();

    /** Read the current dutycycle
     *
     * @returns the dutycycle as a percentage, represented between 0.0-1.0
     */
    float dutycycle();

private:
    InterruptIn _p;
    Timer       _t;
    float       _pulsewidth, _period;

    void rise();

    void fall();
};

class UsainControl
{
    static const PinName RECEIVER_THROTTLE_PIN = PF_2;
    static const PinName RECEIVER_STEER_PIN    = PF_1;
    static const PinName MOTOR_LEFT_OUT_PIN    = PA_15;
    static const PinName MOTOR_RIGHT_OUT_PIN   = PB_1;

public:
    typedef enum
    {
        MODE_RC,
        MODE_UC
    }                    mode_t;

    typedef enum
    {
        MOTOR_LEFT,
        MOTOR_RIGHT
    }                    motor_t;

    UsainControl();

    void set_mode(mode_t mode);

    mode_t get_mode() const;

    void set_motor(motor_t motor, float duty_cycle);

private:
    // current driving mode
    mode_t _mode;

    // when controlled with rc controller
    PwmIn  _motor_throttle_in;
    PwmIn  _motor_steer_in;
    Ticker _rc_handler;

    // when controlled with microcontroller
    PwmOut _motor_left_out;
    PwmOut _motor_right_out;

    void handle_rc();
};

#endif //RC_CONTROL_USAIN_CONTROL_H
