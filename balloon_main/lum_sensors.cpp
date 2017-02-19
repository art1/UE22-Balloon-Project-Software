#include "lum_sensors.h"

//Instances from balloon_main
#ifdef LIGHT_ENABLED
extern SFE_TSL2561 light0;
#endif

#ifdef LIGHT2_ENABLED
extern SFE_TSL2561 light1;
#endif

extern boolean gain;
extern unsigned int ms;

#ifdef LIGHT_ENABLED
void initLightSensor0() {
  light0.begin();
  gain = 0;
  unsigned char time = 3;
  light0.setTiming(gain,time,ms);
  light0.setPowerUp();
}

float readLightSensor0() {
  unsigned int data0, data1;
 
  if (light0.getData(data0,data1))
  {
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    good = light0.getLux(gain,ms,data0,data1,lux);
    if (good) {
      return (float) lux;
    } else {
      return -1.f;
    }
  }
  else
  {
    byte error = light0.getError();
    return -1.f;
  }
  return -1.f;
}
#endif

#ifdef LIGHT2_ENABLED
void initLightSensor1() {
  light1.begin(TSL2561_ADDR_0); //0x29
  gain = 0;
  unsigned char time = 3;
  light1.setTiming(gain,time,ms);
  light1.setPowerUp();
}

float readLightSensor1() {
  unsigned int data0, data1;
 
  if (light1.getData(data0,data1))
  {
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    good = light1.getLux(gain,ms,data0,data1,lux);
    if (good) {
      return (float) lux;
    } else {
      return -1.f;
    }
  }
  else
  {
    byte error = light1.getError();
    return -1.f;
  }
  return -1.f;
}
#endif

