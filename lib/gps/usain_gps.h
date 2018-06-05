//
// Created by Bas on 23-5-2018.
//

#ifndef BLINKY_USAIN_GPS_H
#define BLINKY_USAIN_GPS_H

#include <mbed.h>
#include "drv_gps.h"





class UsainGPS
{
public:
    UsainGPS();

    ~UsainGPS();

    uint8_t init();

    int get_gps_message(AdafruitUltimateGPS::gprmc_data_t &dest);

    void calculate_distance(double dest_latitude, double dest_longitude, double *distance_cm,
                            double *bearing_degrees);


    bool data_received();

private:
    void update();

    Thread _update;

    AdafruitUltimateGPS _gps;


};

#endif //BLINKY_USAIN_GPS_H

