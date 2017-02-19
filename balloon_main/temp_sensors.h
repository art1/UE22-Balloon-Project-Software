#ifndef temp_sensors_h
#define temp_sensors_h
#include "basic.h"

void initMCPSensor0();
void initMCPSensor1();
float readMCPSensor0();
float readMCPSensor1();
void initDallasSensor();
float readDallasSensor();
void initBMPSensor();
pressure_data readBMPSensor_pressure();
void initMSSensor();
pressure_data readMSSensor();

#endif
