#include <SD.h>
#include "basic.h"
class SDCard{
public:
  void init();
  void writeTestString();
  void writeToSD(dataToSD d);
private:
  const int chipSelect = 8;
};
