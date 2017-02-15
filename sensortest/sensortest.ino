#include <SparkFunTSL2561.h>
#include <Adafruit_MCP9808.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

SFE_TSL2561 light = SFE_TSL2561();
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
OneWire ds(ONE_WIRE_BUS);
DallasTemperature Dallastempsensor(&oneWire);



boolean gain = 0;     
unsigned int ms = 100; 
float lightsensval = 0.f; 
float MCPtempval = 0.f;
float Dallastempval = 0.f;
byte addr[8];

void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

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
    Serial.println("Lux: " + String(good) + " " + String(lux) + " " + String(data0) + ", " + String(data1));
    if (good) {
      return (float) lux;
    } else {
      return -1.f;
    }
  }
  else
  {
    byte error = light.getError();
    printError(error);
    return -1.f;
  }
  return -1.f;
}

void initMCPSensor() {
    if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  }
}

float readMCPSensor() {
  return (float) tempsensor.readTempC();
}

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



void setup() {
  Serial.begin(9600);
  Serial.println("Sensor test");

  initLightSensor();
  initMCPSensor();
  initDallasSensor();
}

void loop() {
  ms = 100;
  light.manualStart();
  tempsensor.shutdown_wake(1);
  delay(ms);
  light.manualStop();
  tempsensor.shutdown_wake(0);

  lightsensval = readLightSensor();
  MCPtempval = readMCPSensor();
  Dallastempval = readDallasSensor();

  Serial.print("Light: ");
  Serial.print(lightsensval);
  Serial.println(" ");

  Serial.print("MCP: ");
  Serial.print(MCPtempval);
  Serial.println(" ");

  Serial.print("Dallas: ");
  Serial.print(Dallastempval);
  Serial.println(" ");
 
}


