#ifndef BluetoothDetector_h
#define BluetoothDetector_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

class BluetoothDetector : public Detector
{
public:
  BluetoothDetector(MFRC522 *mfrc522);
  bool RfidCardIsPresent();
  void getPartArray(byte partArray[]); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
private:
  MFRC522 mfrc522;
};

#endif
