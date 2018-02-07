#ifndef RfidDetector_h
#define RfidDetector_h

#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

struct RfidDetectorState
{
  byte partArray[8];
  bool rfIdIsPresent = false;
};
class RfidDetector : public Component<RfidDetectorState>
{
public:
  RfidDetector(MFRC522 *mfrc522);
  bool RfidCardIsPresent();
  RfidDetectorState loop(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
private:
  RfidDetectorState state;
  MFRC522 mfrc522;
};

#endif
