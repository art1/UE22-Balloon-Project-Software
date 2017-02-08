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



#endif
