//
// Created by Jamie on 29-5-2018.
//

#ifndef RC_CONTROL_USAIN_LED_H
#define RC_CONTROL_USAIN_LED_H

#include <mbed.h>

class UsainLED
{
 public:
  typedef enum
  {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_NO
  } color_t;

  typedef enum
  {
    STANDBY,
    LOADING,
    INTERACTIVE,
    ERROR,
    NO
  } pattern_t;

  UsainLED();

  static void set_color(color_t color);

  static void set_pattern(UsainLED::pattern_t pattern);

 private:
  static void routine();

  static Thread led_thread;
  static DigitalOut led_red;
  static DigitalOut led_green;
  static DigitalOut led_blue;

  static pattern_t current_pattern;
};

#endif //RC_CONTROL_USAIN_LED_H
