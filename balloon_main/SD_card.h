#ifndef SD_card_h
#define SD_card_h

#include <SD.h>
#include "basic.h"
class SDCard{
public:
  void init();
  void writeHeader(String fn);
  void writeTestString();
  void writeToSD(dataToSD d, String fn);
  const String filename = "test2.txt";
private:
  const int chipSelect = 8;  
};

#endif