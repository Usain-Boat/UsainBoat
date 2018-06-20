//
// Created by Bas on 19-6-2018.
//

#ifndef BLINKY_USAINMONITOR_H
#define BLINKY_USAINMONITOR_H

#include "mbed.h"

#define V_REF 3.28F

class Usainmonitor{
public:
    Usainmonitor(PinName pin_number, float shunt_value): _adcpin(pin_number) ,_shunt_value(shunt_value){}

    ~Usainmonitor() {}

    float get_shunt_voltage()
    {
        return (_adcpin.read() * V_REF);
    }

    float get_shunt_resistor_value()
    {
        return _shunt_value;
    }

    float get_voltage()
    {
        return V_REF;
    }

    float get_current()
    {
        return (_adcpin.read() * V_REF) / _shunt_value;
    }
    float get_power()
    {
        return V_REF * get_current();
    }

private:
    AnalogIn _adcpin;
    const float _shunt_value;
};

#endif //BLINKY_USAINMONITOR_H