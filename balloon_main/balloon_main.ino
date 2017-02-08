
#include "basic.h"
#include "ahrs.h"
#include "SD_Card.h"

#ifdef IMU_ENABLED
AHRS ahrs;
#endif

#ifdef SD_ENABLED
SDCard sd;
#endif

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
  #endif
  
  timer=millis();
  delay(20);
}



void loop() //Main Loop
{
  if((millis()-timer)>=20)  // Main loop runs at 50Hz
  {
    timer_old = timer;
    timer=millis();


#ifdef IMU_ENABLED
    ahrs.ahrs_fetchData(timer,timer_old);
    filtered_data f = ahrs.getFilteredData();
    raw_data r = ahrs.getRawData();

      Serial.print("RPY:");
      Serial.print(ToDeg(f.roll));
      Serial.print(",");
      Serial.print(ToDeg(f.pitch));
      Serial.print(",");
      Serial.print(ToDeg(f.yaw));
      Serial.println();
#endif




  }

    if((millis()-timer2)>=200)  // SD loop runs at 5Hz
  {
        timer_old2 = timer2;
        timer2=millis();
    
#ifdef SD_ENABLED
        sd.writeString();
#endif    
  }

}
