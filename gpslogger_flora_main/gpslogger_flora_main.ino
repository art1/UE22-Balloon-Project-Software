//#include <Adafruit_GPS.h>

#include <SoftwareSerial.h>
//#include "Adafruit_GPS_mod.h"
#include "Adafruit_GPS.h"


Adafruit_GPS GPS(&Serial1);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

#define GPS_SYNC_PIN  10
#define LED 7

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
boolean syncNow = true;

void setup()  
{    
  //while (!Serial);
  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS logging start test!");


  GPS.begin(9600);
  //GPS.sendCommand(PMTK_SET_BAUD_9600);
  // You can adjust which sentences to have the module emit, below
  // Default is RMC + GGA
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Default is 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  //  GPS.sendCommand(PMTK_LOCUS_ERASE_FLASH);

  int i=0;
  while(i<5){GPS.sendCommand("$PMTK187,1,5*38");i++;}
  
  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);


  

  delay(500);
  while (true) {
    Serial.print("Starting logging....");
    if (GPS.LOCUS_StartLogger()) {
      Serial.println(" STARTED!");
      break;
    } else {
      Serial.println(" no response :(");
    }
  }
  pinMode(GPS_SYNC_PIN,OUTPUT);
  pinMode(LED, OUTPUT);
  delay(50);
  digitalWrite(GPS_SYNC_PIN,LOW);
  digitalWrite(LED, LOW);
}



/******************************************************************/
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);  
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
uint32_t timer = millis();
void loop()                     // run over and over again
{


    delay(500);
    /*GPS.LOCUS_ReadStatus();
    Serial.print("intervall:"); Serial.println(GPS.LOCUS_interval);
    Serial.print("mode:"); Serial.println(GPS.LOCUS_mode);
    */
    Serial.print("gps sync: "); Serial.println(digitalRead(GPS_SYNC_PIN));

  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA())){   // this also sets the newNMEAreceived() flag to false
      Serial.println("failed to parse!");
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }
  }
  if(GPS.fix){
    if(syncNow){
      digitalWrite(GPS_SYNC_PIN,HIGH);
      digitalWrite(LED,HIGH);
      Serial.println("ISR triggered, Fix found");
      syncNow = false;
    }
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();
  
  
  // approximately every 2 seconds or so, print out the current stats
  //if (millis() - timer > 2000) {
  if (millis() - timer > 2000) { 
    timer = millis(); // reset the timer    
    
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (true) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
}

