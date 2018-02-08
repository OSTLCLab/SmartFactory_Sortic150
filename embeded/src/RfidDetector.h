#ifndef RfidDetector_h
#define RfidDetector_h

#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

class RfidDetector : public Component<byte *>
{
public:
  RfidDetector(MFRC522 &mfrc522) : mfrc522{mfrc522} {}

protected:
  byte *loop();

private:
  MFRC522 &mfrc522;
};

#endif
