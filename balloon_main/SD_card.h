#include <SD.h>

class SDCard{
public:
  void init();
  void writeString();
private:
  const int chipSelect = 8;
};
