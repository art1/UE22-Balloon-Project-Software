
#include "basic.h"
#include "ahrs.h"

AHRS ahrs;

long timer=0;   //general purpuse timer
long timer_old;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  ahrs.ahrs_init();

  timer=millis();
  
  delay(20);
}



void loop() //Main Loop
{
  if((millis()-timer)>=20)  // Main loop runs at 50Hz
  {
    timer_old = timer;
    timer=millis();

    ahrs.ahrs_fetchData(timer,timer_old);
    filtered_data f = ahrs.getFilteredData();
    raw_data r = ahrs.getRawData();

      Serial.print("ANG:");
      Serial.print(ToDeg(f.roll));
      Serial.print(",");
      Serial.print(ToDeg(f.pitch));
      Serial.print(",");
      Serial.print(ToDeg(f.yaw));
      Serial.println();
      /*
      Serial.print(",AN:");
      Serial.print(r.gx);  //(int)read_adc(0)
      Serial.print(",");
      Serial.print(r.gy);
      Serial.print(",");
      Serial.print(r.gz);
      Serial.print(",");
      Serial.print(r.ax);
      Serial.print (",");
      Serial.print(r.ay);
      Serial.print (",");
      Serial.print(r.az);
      Serial.print(",");
      Serial.print(r.mx);
      Serial.print (",");
      Serial.print(r.my);
      Serial.print (",");
      Serial.print(r.mz);
      Serial.println();
  */


  }

}
