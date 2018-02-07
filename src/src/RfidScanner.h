#ifndef rfidscanner_h
#define rfidscanner_h

#include <MFRC522.h>
#include <Sensor.h>

class RfidScanner : public Sensor<uint8_t *>
{
public:
  RfidScanner(MFRC522 &scanner) : scanner{scanner}
  {
  }
  bool ready();
  uint8_t *get();

private:
  MFRC522 &scanner;
};

#endif