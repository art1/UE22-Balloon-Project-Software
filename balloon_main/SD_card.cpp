#include "SD_card.h"


/* Initializes SDCard instance. Watch out for PIN setting
 */
void SDCard::init(){
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    return;
  }
}

/* Header used at start to write to SD
 *  @Input: String: filename specifying the name and format (.txt, .csv)
 */
void SDCard::writeHeader(String fn){
  String dataString = "Yaw,Pitch,Roll,temp0,temp1,temp2,temp3,temp4,temp5,press,humid,lum0,lum1,lum2";  
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
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fn);
  }
}
