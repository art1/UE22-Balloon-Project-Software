/*
MinIMU-9-Arduino-AHRS 
---------------------
MODIFIED, REDUCED AND RESTRUCTURED CODE FOR M2TSI BALLOON PROJECT AT 
UPS TOULOUSE by A.Scharf
---------------------
Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)
Copyright (c) 2011-2016 Pololu Corporation.
http://www.pololu.com/
MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/
sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/
MinIMU-9-Arduino-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.
MinIMU-9-Arduino-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.
You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ahrs_h
#define ahrs_h
#include "IMU.h"
#include "basic.h"


class AHRS
{
  public:
    AHRS();
    void ahrs_init();
    void ahrs_fetchData(long timer, long timer_old);
    filtered_data getFilteredData();
    raw_data getRawData();
  private:
    IMU imu;
    float inttime;
    int counter;





};


#endif
