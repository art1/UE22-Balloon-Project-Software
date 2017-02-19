#include "temp_sensors.h"

byte addr[8];
#ifdef MCP_ENABLED
extern Adafruit_MCP9808 tempsensor0;
#endif

#ifdef DALLAS_ENABLED
extern OneWire oneWire;
extern OneWire ds;
extern DallasTemperature Dallastempsensor;
#endif

#ifdef BMP_ENABLED
extern SFE_BMP180 pressure;
#define ALTITUDE 150.0 // Altitude of Toulouse
#endif

#ifdef MS_ENABLED
extern MS5611 ms5611;
#endif

#ifdef MCP2_ENABLED
#define MCP9808_I2CADDR_A1             0x1A
extern Adafruit_MCP9808 tempsensor1;
#endif


#ifdef MCP_ENABLED
void initMCPSensor0() {
  if (!tempsensor0.begin()) {
    Serial.println("Couldn't find first MCP9808!");
  }
}

float readMCPSensor0() {
  float c = tempsensor0.readTempC();
  return c;
}
#endif

#ifdef MCP2_ENABLED
void initMCPSensor1() {
  if (!tempsensor1.begin(MCP9808_I2CADDR_A1)) {
    Serial.println("Couldn't find second MCP9808!");
  }
}

float readMCPSensor1() {
  float c = tempsensor1.readTempC();
  return c;
}
#endif

#ifdef DALLAS_ENABLED
void initDallasSensor() {
  if ( !ds.search(addr)) {
    ds.reset_search();
  }
}

float readDallasSensor() {
  //returns the temperature from one DS18S20 in DEG Celsius
  
  byte data[12];
  
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }
  
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  
  delay(750); // Wait for temperature conversion to complete
  
  byte present = ds.reset();
  ds.select(addr);  
  ds.write(0xBE); // Read Scratchpad
  
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  float TemperatureSum = ((float)((data[1] << 8) | data[0])) * 0.0625 + 21;
  
  return TemperatureSum;
}
#endif

#ifdef BMP_ENABLED
void initBMPSensor(){
  if(!pressure.begin()){
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("BMP180 init fail");
  }
}


pressure_data readBMPSensor_pressure(){
  char status;
  double T,P,p0,a;
  pressure_data d;
  // You must first get a temperature measurement to perform a pressure reading.
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.  
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      d.T = (float) T;
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.  
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);
        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          d.P = (float) P;
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb        
          p0 = pressure.sealevel(P,ALTITUDE);
          a = pressure.altitude(P,p0);
          d.alt = (float) a;
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  return d;
}
#endif


#ifdef MS_ENABLED
void initMSSensor(){
  if(!ms5611.begin()) Serial.println("MS5611 init fail");
}

pressure_data readMSSensor(){
  pressure_data d;
  d.T = (float) ms5611.readTemperature();
  d.P = (ms5611.readPressure() / 100.0f);
  d.alt = (float) ms5611.getAltitude((double) d.P*100.0);
  return d;
}
#endif
