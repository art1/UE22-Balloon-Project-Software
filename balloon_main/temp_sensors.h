#ifndef temp_sensors_h
#define temp_sensors_h
#include "basic.h"

void initMCPSensor();
float readMCPSensor();
void initDallasSensor();
float readDallasSensor();
void initBMPSensor();
pressure_data readBMPSensor_pressure();
void initMSSensor();
pressure_data readMSSensor();

#endif
