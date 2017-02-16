#include "temp_sensors.h"

byte addr[8];
#ifdef MCP_ENABLED
extern Adafruit_MCP9808 tempsensor;
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

#ifdef MCP_ENABLED
void initMCPSensor() {
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  }
}

float readMCPSensor() {
  float c = tempsensor.readTempC();
  /* delay(250);
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
  
  delay(100);
  
  tempsensor.shutdown_wake(0);*/
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


BMP180_data readBMPSensor_pressure(){
  char status;
  double T,P,p0,a;
  BMP180_data d;
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
      // Print out the measurement:
      /*Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");*/
      d.T = (float) T;
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.
      
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        //Serial.println("Stat1: " + status);
        delay(status);
        
        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.
        
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          /*Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");*/
          d.P = (float) P;
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb
          
          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          /*Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");*/
          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.
          
          a = pressure.altitude(P,p0);
          /*Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");*/
          d.alt = (float) a;
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  
  //delay(5000);  // Pause for 5 seconds.
  return d;
}
#endif
