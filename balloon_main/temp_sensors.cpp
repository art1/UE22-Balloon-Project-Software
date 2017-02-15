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

#ifdef MCP_ENABLED
void initMCPSensor() {
    if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  }
}

float readMCPSensor() {
  return (float) tempsensor.readTempC();
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