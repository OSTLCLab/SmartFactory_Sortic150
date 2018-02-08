#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <RfidDetector.h>

RfidDetector::RfidDetector(MFRC522 *RfidDetector)
{
  state = {};
  mfrc522 = *RfidDetector;
  mfrc522.PCD_Init();
}

RfidDetectorState RfidDetector::loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    state.cardDetected = false;
    return state;
  }

  byte blockSize = 18;
  MFRC522::StatusCode status{mfrc522.MIFARE_Read(0, state.partArray, &blockSize)};
  Serial.println("Actual RFIDScanner status[" + String(status) + "] Status Ok[" + String(MFRC522::STATUS_OK) + "]");
  state.cardDetected = status == MFRC522::STATUS_OK;

  return state;
}
