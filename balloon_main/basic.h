#ifndef basic_h
#define basic_h
#include "Arduino.h"
#include "Wire.h"


// AHRS and IMU stuff
//#define IMU_ENABLED
//#define IMU_DEBUG_OUTPUT
#define SD_ENABLED

struct raw_data{
  int gx;
  int gy;
  int gz;
  int ax;
  int ay;
  int az;
  int mx;
  int my;
  int mz;
};

struct filtered_data{
  float yaw;
  float pitch;
  float roll;
};

struct dataToSD{
  float yaw;
  float pitch;
  float roll;
  float temp0;temp1;temp2;temp3;temp4;temp5;
  float press;
  float humid;
  float lum0;lum1;lum2;
  
  dataToSD(): yaw(0),pitch(0),roll(0),temp0(0),temp1(0),temp2(0),temp3(0)
              temp4(0),temp5(0),press(0),humid(0),lum(0),lum1(0),lum2(0) {}
              
              
  String toString(){
    short dec = 3;
    return String(yaw, dec) + String(pitch, dec) + String(roll, dec) + String(temp0, dec)
    + String(temp1, dec) + String(temp2, dec) + String(temp3, dec) + String(temp4, dec);
    + String(temp4, dec) + String(press, dec) + + String(humid, dec) + + String(lum0, dec)
    + String(lum1, dec) + String(lum2, dec);
  }
};


#endif
