#ifndef DetectorSeverin_h
#define DetectorSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

class DetectorSeverin : public Detector
{
  public:
    DetectorSeverin(MFRC522 *tempPartDetector);
    bool RfidCardIsPresent();
    void getPartArray(byte partArray[]); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
  private:
    MFRC522 PartDetector;
};

#endif
