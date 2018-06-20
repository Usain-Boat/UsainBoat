//
// Created by Bas on 23-5-2018.
//

#include "usain_gps.h"
#include "drv_gps.h"

#define pi 3.141592653589793238462

UsainGPS::UsainGPS() {}

uint8_t UsainGPS::init()
{
  uint8_t return_value = 0;
//    int status = 0;
  int status = _gps.coldstart();
  if (status)
  {
    return_value |= 0x01;
  }
  Timer delaytje;
  delaytje.reset();
  delaytje.start();
  while (_gps.setbaudrateto115200() && delaytje.read() < 1) { wait_ms(1); }
  delaytje.reset();
  delaytje.start();
  while (delaytje.read() < 1) { wait_ms(1); }
  delaytje.reset();
  delaytje.start();
  while (_gps.setupdaterate((char *) "100") && delaytje.read() < 1) { wait_ms(1); }

  if (status)
  {
    return_value |= 0x02;
  }
  status = _gps.onlyreceivegprmcdata();
  if (status)
  {
    return_value |= 0x04;
  }

  if (_update.start(callback(this, &UsainGPS::update)) == osOK)
  {
  };

  return return_value;
}

int UsainGPS::get_gps_message(AdafruitUltimateGPS::gprmc_data_t &dest)
{
  _gps.GetLastGprmcData(&dest);
  if (!_gps.ReceievedNewGPRMC())
  {
    return -1;
  }
  _gps.ReceievedNewGPRMC(false);
  if (*dest.validity == 'V')
  {
    return -2; // data not valid
  }
  return 0;
}

void UsainGPS::get_average_gps(double *latitude, double *longitude)
{
  _gps.getaveragelocation(longitude, latitude);
}

//haversine method
void UsainGPS::calculate_distance(double dest_latitude, double dest_longitude, double *distance_cm,
                                  double *bearing_degrees)
{
  double home_latitude, home_longitude;
  _gps.getaveragelocation(&home_longitude, &home_latitude);

  const double R = 6378.137e5;
  double rlat1 = home_latitude * (pi / 180);
  double rlat2 = dest_latitude * (pi / 180);
  double dlon = (dest_longitude - home_longitude) * (pi / 180);
  double dlat = (dest_latitude - home_latitude) * (pi / 180);
  double a = pow(sin(dlat / 2.0), 2.0) + cos(rlat1) * cos(rlat2) * pow(sin(dlon / 2.0), 2.0);
  //double c = 2 * asin(min(one,sqrt(a)));
  double c = 2 * atan2(sqrt(a), sqrt(1.0 - a));

//    *distance_cm = R * c;
  double rad_bearing = atan2(sin(dlon) * cos(rlat2),
                             cos(rlat1) * sin(rlat2)
                                 - sin(rlat1) * cos(rlat2) * cos(dlon));

  *distance_cm = (R * c);
  *bearing_degrees = rad_bearing * (180 / pi);
}

bool UsainGPS::data_received()
{
  return _gps.ReceievedNewGPRMC();
}

void UsainGPS::update()
{
  AdafruitUltimateGPS::gprmc_data_t gps_data;

  while (1)
  {
    _gps.parsedata();
    if (_gps.ReceievedNewGPRMC() && _collision_callback)
    {
      _gps.GetLastGprmcData(&gps_data);
      _collision_callback.call(gps_data);
    }

  }
}



