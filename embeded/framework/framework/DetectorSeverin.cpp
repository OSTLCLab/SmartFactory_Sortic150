#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#include "DetectorSeverin.h"

DetectorSeverin::DetectorSeverin(MFRC522 *tempPartDetector) : Detector() {
  PartDetector = tempPartDetector;
  PartDetector.PCD_Init();
}

bool  DetectorSeverin::RfidCardIsPresent() {
  if (PartDetector.PICC_IsNewCardPresent())
  {
    return true;
  }
  return false;
}

void  DetectorSeverin::getPartArray(byte partArray[]) {
  //Example: partArray[0] = 1;
  byte blockAddr = 0;
  byte bufferSize = 8;
  byte outputSize = 8;
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode) PartDetector.MIFARE_Read(blockAddr, partArray, &bufferSize);
  /* Error handling:
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(PartDetector.GetStatusCodeName(status));
  }
  */
}
