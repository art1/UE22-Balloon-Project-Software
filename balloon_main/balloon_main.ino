#include "basic.h"
#include "ahrs.h"
#include "SD_Card.h"
#include "lum_sensors.h"
#include "temp_sensors.h"
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

#ifdef HUMID_ENABLED
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#endif

#ifdef BMP_ENABLED
SFE_BMP180 pressure;
#endif

#ifdef MS_ENABLED
MS5611 ms5611;
#endif

//MCP and Dallas variables
float lightsensval = 0.f;
float MCPtempval = 0.f;
float Dallastempval = 0.f;
boolean gain;
unsigned int ms = 1000;

// HTU variables
float humidity = 0.0f;
float htu_temp = 0.0f;

// GPS Sync variables
boolean gpsSynced = false;
boolean gpsFound = false;

dataToSD d;


unsigned long timer=0, timer_sd=0, timer_dallas=0, timer_mcp=0, timerGPS=0;   //general purpuse timer
unsigned long timer_old, timer_old_sd, timer_old_dallas, timer_old_mcp, timerGPS_old;

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("initializing Balloon...."));
  Wire.begin();

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

  #ifdef HUMID_ENABLED
  htu.begin();
  #endif

  #ifdef BMP_ENABLED
  pressure.begin();
  #endif

  #ifdef IMU_ENABLED
  ahrs.ahrs_init();
  delay(200);
  #endif

  #ifdef GPS_SYNC_ENABLED
  pinMode(GPS_SYNC_PIN,INPUT);
  attachInterrupt(digitalPinToInterrupt(GPS_SYNC_PIN),gpsSync_ISR,HIGH);
  #endif

  #ifdef MS_ENABLED
  initMSSensor();
  #endif

  timer=millis();
}


void gpsSync_ISR(){
  Serial.println("Interrupt received!");
  #ifdef GPS_SYNC_ENABLED
  gpsFound = true;
  #endif
}


void loop() //Main Loop
{
  pressure_data bmp;
  pressure_data ms1;
  if((millis()-timer)>=20)  // AHRS loop runs at 50Hz
  {
    //Serial.println(String(millis() - timer_old));
    timer_old = timer;
    timer=millis();

    #ifdef IMU_ENABLED
    ahrs.ahrs_fetchData(timer,timer_old);

    #ifdef IMU_DEBUG_OUTPUT
    filtered_data filt = ahrs.getFilteredData();
    raw_data raw = ahrs.getRawData();
    Serial.print("RPY:");
    Serial.print(ToDeg(filt.roll));
    Serial.print(",");
    Serial.print(ToDeg(filt.pitch));
    Serial.print(",");
    Serial.print(ToDeg(filt.yaw));
    Serial.println();
    #endif
    #endif
    //Serial.println("Test1");
  }




  if((millis()-timer_sd)>=500)  // SD loop runs at 2Hz
  {
    timer_old_sd = timer_sd;
    timer_sd=millis();


    d.lum0 = lightsensval;
    d.humid = humidity;
    d.temp3 = htu_temp;
    d.temp0 = MCPtempval;

    #ifdef BMP_ENABLED
    bmp = readBMPSensor_pressure();
    d.pressureDataToSDStruct(bmp,FALSE);
    #endif

    #ifdef MS_ENABLED
    ms1 = readMSSensor();
    d.pressureDataToSDStruct(ms1,TRUE);
    #endif

    #ifdef IMU_ENABLED
    d.filterDataToSDStruct(ahrs.getFilteredData());
    d.rawDataToSDStruct(ahrs.getRawData());
    #endif

    #ifdef SD_ENABLED
    #ifdef GPS_SYNC_ENABLED
    if(gpsFound){
      if(!gpsSynced){
        sd.writeGPSSync(sd.filename,millis());  
        gpsSynced = true;
      }     
    }
    #endif
    sd.writeToSD(d, sd.filename); //writes Data to specified File
    #endif

    #ifdef DEBUG_OUTPUT
    Serial.println(d.toString());
    #endif
  }

  if((millis()-timer_mcp)>=100)  // Measurement loop runs at 10Hz
  {

    timer_old_mcp = timer_mcp;
    timer_mcp=millis();

    #ifdef MCP_ENABLED
    MCPtempval = readMCPSensor();
    delay(250);
    #endif

    #ifdef LIGHT_ENABLED
    light.manualStart();
    #endif

    #ifdef MCP_ENABLED
    tempsensor.shutdown_wake(1);
    #endif

    ms = 100;
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

    #ifdef DALLAS_ENABLED
    Dallastempval = readDallasSensor();
    #endif

    #ifdef HUMID_ENABLED
    humidity = htu.readHumidity();
    htu_temp = htu.readTemperature();
    #endif


  }

}
