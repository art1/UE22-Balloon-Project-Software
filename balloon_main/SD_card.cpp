#include "SD_card.h"
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

void SDCard::writeTestString(){
  // make a string for assembling the data to log:
  String dataString = "";

  // read three values and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogPin;
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("test.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
