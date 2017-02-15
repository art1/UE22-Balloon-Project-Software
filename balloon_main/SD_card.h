#ifndef SD_card_h
#define SD_card_h

#include <SD.h>
#include "basic.h"
class SDCard{
public:
  void init();
  void writeHeader(String fn);
  void writeGPSSync(String fn, unsigned long m);
  void writeToSD(dataToSD d, String fn);
  const String filename = "test2.csv";
private:
  const int chipSelect = 8;
  void blink();
  bool led_state = 1;
};

#endif
