#include "lum_sensors.h"

//Instances from balloon_main
#ifdef LIGHT_ENABLED
extern SFE_TSL2561 light;
#endif

extern boolean gain;
extern unsigned int ms;

#ifdef LIGHT_ENABLED
void initLightSensor() {
  light.begin();
  gain = 0;
  unsigned char time = 3;
  light.setTiming(gain,time,ms);
  light.setPowerUp();
}

float readLightSensor() {
  unsigned int data0, data1;
 
  if (light.getData(data0,data1))
  {
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    good = light.getLux(gain,ms,data0,data1,lux);
    if (good) {
      return (float) lux;
    } else {
      return -1.f;
    }
  }
  else
  {
    byte error = light.getError();
    return -1.f;
  }
  return -1.f;
}
#endif
