#include "SD_card.h"


/* Initializes SDCard instance. Watch out for PIN setting
 */
void SDCard::init(){
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  pinMode(22, OUTPUT); //LED PIN

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    // don't do anything more.
    return;
  }
}

/* Header used at start to write to SD
 *  @Input: String: filename specifying the name and format (.txt, .csv)
 */
void SDCard::writeHeader(String fn){
  String dataString = "Yaw,Pitch,Roll,Gx,Gy,Gz,Ax,Ay,Az,Mx,My,Mz,temp0,temp1,temp2,temp3,temp4,press0,press1,alt0,alt1,humid,lum0,lum1";
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fn, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fn);
  }
}

/* Header used at start to write to SD
 *  @Input: String: filename specifying the name and format (.txt, .csv),
 *          unsigned long: current arduino time in milliseconds
 */
void SDCard::writeGPSSync(String fn, unsigned long m){
  String dataString = ";GPS Fix Interrupt at arduino time " + String(m,DEC);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fn, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening " + fn);
  }
}

/* Main write method used for generated data.
 * @Input: dataToSD: Struct containing necessary values
 *         String: filename specifying the name and format (.txt, .csv)
 */
void SDCard::writeToSD(dataToSD d, String fn){
  String dataString = d.toString();
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fn, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
    this->blink();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fn);
  }
}

/* Blinks LED on PIN22, in order to signalize that Data is
 * being written to the SD
 */
void SDCard::blink(){
  if(led_state){
    digitalWrite(22, HIGH);   // turn the LED on (HIGH is the voltage level)
    led_state = 0;
  }else{
    digitalWrite(22, LOW);   // turn the LED off by making the voltage LOW
    led_state = 1;
  }
}
