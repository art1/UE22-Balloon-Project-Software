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

#include "ahrs.h"

AHRS::AHRS(){
  IMU imu;
  inttime = 0.02;
  counter = 0;
}


/**
 * initializes the IMU and calibrates the Accl and Gyro (do not touch while calibrating!)
 */
void AHRS::ahrs_init(){
  Serial.print("\nInitializing IMU...");
  imu.IMUdata_Init();  

  delay(1500);

  imu.Accel_Init();
  imu.Compass_Init();
  imu.Gyro_Init();
  Serial.print("..done!\n");
  delay(20);
  Serial.print("Calibrating IMU...");
  for(int i=0;i<32;i++)    // We take some readings...
    {
    imu.Read_Gyro();
    imu.Read_Accel();
    for(int y=0; y<6; y++)   // Cumulate values
      imu.AN_OFFSET[y] += imu.AN[y];
    delay(20);
    }

  for(int y=0; y<6; y++)
    imu.AN_OFFSET[y] = imu.AN_OFFSET[y]/32;

  imu.AN_OFFSET[5]-=GRAVITY*imu.SENSOR_SIGN[5];
  delay(2000);
  counter=0;
  Serial.print("..done!\n");


}


/**
 * fetches data from sensors and fuses them
 * input arguments:
 *    long timer      current time in milliseconds
 *    long timer_old  timer from previous loop cycle
 */
void AHRS::ahrs_fetchData(long timer, long timer_old){
  counter++;
  if (timer>timer_old)
    {
      inttime = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
      if (inttime > 0.2)
        imu.set_integration(0); // ignore integration times over 200 ms
       else imu.set_integration(inttime);
    }
    else
      imu.set_integration(0);

    // *** DCM algorithm
    // Data adquisition
    imu.Read_Gyro();   // This read gyro data
    imu.Read_Accel();     // Read I2C accelerometer

    if (counter > 5)  // Read compass data at 10Hz... (5 loop runs)
    {
      counter=0;
      imu.Read_Compass();    // Read I2C magnetometer
      imu.Compass_Heading(); // Calculate magnetic heading
    }

    
    // Calculations...
    imu.Matrix_update();
    imu.Normalize();
    imu.Drift_correction();
    imu.Euler_angles();
}

/**
 * return raw data as a struct
 */
raw_data AHRS::getRawData(){
  return imu.getRawData();
}


/**
 * returns filtered data as struct, yaw, pitch and roll
 */
filtered_data AHRS::getFilteredData(){
  return imu.getFilteredData();
}


