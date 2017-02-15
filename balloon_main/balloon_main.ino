
#include "basic.h"
#include "ahrs.h"
#include "SD_Card.h"
#include "lum_sensors.h"

#ifdef IMU_ENABLED
AHRS ahrs;
#endif

#ifdef SD_ENABLED
SDCard sd;
#endif

#ifdef DALLAS_ENABLED
OneWire oneWire(ONE_WIRE_BUS);
OneWire ds(ONE_WIRE_BUS);
DallasTemperature Dallastempsensor(&oneWire);
#endif

#ifdef MCP_ENABLED
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
#endif

#ifdef LIGHT_ENABLED
SFE_TSL2561 light = SFE_TSL2561();
#endif

//MCP and Dallas variables
float lightsensval = 0.f;
float MCPtempval = 0.f;
float Dallastempval = 0.f;
byte addr[8];
boolean gain;
unsigned int ms = 1000;


long timer=0, timer2=0;   //general purpuse timer
long timer_old, timer_old2;


void setup()
{
  Serial.begin(115200);
  Serial.println("initializing Balloon....");
  Wire.begin();

  #ifdef IMU_ENABLED
  ahrs.ahrs_init();
  #endif

  #ifdef SD_ENABLED
  sd.init();
  sd.writeHeader(sd.filename);
  #endif

  #ifdef LIGHT_ENABLED
  initLightSensor();
  #endif

  #ifdef MCP_ENABLED
  initMCPSensor();
  #endif

  #ifdef DALLAS_ENABLED
  initDallasSensor();
  #endif


  timer=millis();
  delay(20);
}



void loop() //Main Loop
{
  if((millis()-timer)>=20)  // AHRS loop runs at 50Hz
  {
    timer_old = timer;
    timer=millis();

    #ifdef IMU_ENABLED
    ahrs.ahrs_fetchData(timer,timer_old);

    #ifdef IMU_DEBUG_OUTPUT
    filtered_data filt = ahrs.getFilteredData();
    raw_data raw = ahrs.getRawData();
    Serial.print("RPY:");
    Serial.print(ToDeg(f.roll));
    Serial.print(",");
    Serial.print(ToDeg(f.pitch));
    Serial.print(",");
    Serial.print(ToDeg(f.yaw));
    Serial.println();
    #endif
    #endif
  }



  if((millis()-timer2)>=500)  // SD loop runs at 2Hz
  {

    #ifdef LIGHT_ENABLED
    ms = 100;
    light.manualStart();
    #endif

    #ifdef MCP_ENABLED
    tempsensor.shutdown_wake(1);
    #endif

    delay(ms); //This should be moved

    #ifdef LIGHT_ENABLED
    light.manualStop();
    #endif

    #ifdef MCP_ENABLED
    tempsensor.shutdown_wake(0);
    #endif

    #ifdef LIGHT_ENABLED
    lightsensval = readLightSensor();
    #endif

    timer_old2 = timer2;
    timer2=millis();

    #ifdef SD_ENABLED
    dataToSD d;
    #ifdef IMU_ENABLED
    d.filterDataToSD(ahrs.getFilteredData());
    d.rawDataToSD(ahrs.getRawData());
    #endif
    d.lum0 = lightsensval;
    sd.writeToSD(d, sd.filename); //writes Data to specified File
    #endif
  }

}
