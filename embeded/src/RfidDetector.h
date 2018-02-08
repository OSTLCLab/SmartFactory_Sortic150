#ifndef RfidDetector_h
#define RfidDetector_h

#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

struct RfidDetectorState
{
  byte partArray[8]{};
  bool cardDetected = false;
};
class RfidDetector : public Component<RfidDetectorState>
{
public:
  RfidDetector(MFRC522 *mfrc522);
  RfidDetectorState loop();

private:
  RfidDetectorState state;
  MFRC522 mfrc522;
};

#endif
