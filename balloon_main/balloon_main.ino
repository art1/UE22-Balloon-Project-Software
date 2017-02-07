
#include "basic.h"
#include "ahrs.h"

#ifdef IMU_ENABLED
AHRS ahrs;
#endif

long timer=0;   //general purpuse timer
long timer_old;

void setup()
{
  Serial.begin(115200);
  Serial.println("initializing Balloon....");
  Wire.begin();

  #ifdef IMU_ENABLED
  ahrs.ahrs_init();
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

}
