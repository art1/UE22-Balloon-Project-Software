#include <SD.h>

class SDCard{
public:
  void init();
  void writeTestString();
private:
  const int chipSelect = 8;
};
