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
Adafruit_MCP9808 tempsensor0 = Adafruit_MCP9808();
#endif


#ifdef MCP2_ENABLED
Adafruit_MCP9808 tempsensor1 = Adafruit_MCP9808();
#endif

#ifdef LIGHT_ENABLED
SFE_TSL2561 light0 = SFE_TSL2561();
#endif

#ifdef LIGHT2_ENABLED
SFE_TSL2561 light1 = SFE_TSL2561();
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
float lightsensval0 = 0.f, lightsensval1 = 0.f;
float MCPtempval0 = 0.f, MCPtempval1 = 0.f;
float Dallastempval = 0.f;
boolean gain;
unsigned int ms = 1000;

// HTU variables
float humidity = 0.0f;
float htu_temp = 0.0f;


#ifdef GPS_SYNC_ENABLED
int currentGPS_SyncPinState(){
  return digitalRead(GPS_SYNC_PIN);
}
#endif


dataToSD d;



unsigned long timer=0, timer_sd=0, timer_dallas=0, timer_mcp=0, timerGPS=0;   //general purpuse timer
unsigned long timer_old, timer_old_sd, timer_old_dallas, timer_old_mcp, timerGPS_old;

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");
}


void runIMUIntegration(){
    #ifdef IMU_ENABLED
    if((millis()-timer)>=20)  // AHRS loop runs at 50Hz
    {
      timer_old = timer;
      timer=millis();
      ahrs.ahrs_fetchData(timer,timer_old);
    }
    #endif
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
  initLightSensor0();
  #endif

  #ifdef LIGHT2_ENABLED
  initLightSensor1();
  #endif

  #ifdef MCP_ENABLED
  initMCPSensor0();
  #endif

  #ifdef MCP2_ENABLED
  initMCPSensor1();
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
  digitalWrite(GPS_SYNC_PIN,HIGH);
  #endif

  #ifdef MS_ENABLED
  initMSSensor();
  #endif

  timer=millis();
}


void loop() //Main Loop
{
  pressure_data bmp;
  pressure_data ms1;
  #ifdef IMU_ENABLED

  //runIMUIntegration();

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




  if((millis()-timer_sd)>=500)  // SD loop runs at 2Hz
  {
    timer_old_sd = timer_sd;
    timer_sd=millis();


    d.lum0 = lightsensval0;
    d.lum1 = lightsensval1;
    d.humid = humidity;
    d.temp2 = htu_temp;
    d.temp0 = MCPtempval0;
    d.temp1 = MCPtempval1;


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


    #ifdef GPS_SYNC_ENABLED
    int fix = currentGPS_SyncPinState();
    if(fix == 0) fix = 1;
    else if(fix == 1) fix = 0;
    d.gpsFix = fix;
    #endif

    #ifdef SD_ENABLED
    sd.writeToSD(d, sd.filename); //writes Data to specified File
    #endif

    //runIMUIntegration();

    #ifdef DEBUG_OUTPUT
    Serial.println(d.toString_noIMU());
    #endif
  }

  if((millis()-timer_mcp)>=100)  // Measurement loop runs at 10Hz
  {

    timer_old_mcp = timer_mcp;
    timer_mcp=millis();

    #ifdef MCP_ENABLED
    MCPtempval0 = readMCPSensor0();
    #endif

    #ifdef MCP2_ENABLED
    MCPtempval1 = readMCPSensor1();
    #endif

    //runIMUIntegration();

    #if defined(MCP_ENABLED) || defined(MCP2_ENABLED)
    delay(250);
    #endif

    //runIMUIntegration();

    #ifdef LIGHT_ENABLED
    light0.manualStart();
    #endif

    #ifdef LIGHT2_ENABLED
    light1.manualStart();
    #endif

    #ifdef MCP_ENABLED
    tempsensor0.shutdown_wake(1);
    #endif

    #ifdef MCP2_ENABLED
    tempsensor1.shutdown_wake(1);
    #endif

    ms = 100;
    delay(ms); //This should be moved

    //runIMUIntegration();

    #ifdef LIGHT_ENABLED
    light0.manualStop();
    #endif

    #ifdef LIGHT2_ENABLED
    light1.manualStop();
    #endif

    #ifdef MCP_ENABLED
    tempsensor0.shutdown_wake(0);
    #endif

    #ifdef MCP2_ENABLED
    tempsensor1.shutdown_wake(0);
    #endif

    #ifdef LIGHT_ENABLED
    lightsensval0 = readLightSensor0();
    #endif

    #ifdef LIGHT2_ENABLED
    lightsensval1 = readLightSensor1();
    #endif

    #ifdef DALLAS_ENABLED
    Dallastempval = readDallasSensor();
    #endif

    //runIMUIntegration();

    #ifdef HUMID_ENABLED
    humidity = htu.readHumidity();
    htu_temp = htu.readTemperature();
    #endif


  }

}
