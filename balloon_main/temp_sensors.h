#ifndef temp_sensors_h
#define temp_sensors_h
#include "basic.h"

void initMCPSensor();
float readMCPSensor();
void initDallasSensor();
float readDallasSensor();

#endif
