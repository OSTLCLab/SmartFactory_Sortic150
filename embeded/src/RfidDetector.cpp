#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <RfidDetector.h>

byte *RfidDetector::loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return data;
  }

  Serial.println("Card detected");
  byte blockSize = 18;
  MFRC522::StatusCode status{mfrc522.MIFARE_Read(0, data, &blockSize)};
  Serial.println("Actual RFIDScanner status[" + String(status) + "] Status Ok[" + String(MFRC522::STATUS_OK) + "]");

  state = status == MFRC522::STATUS_OK ? State::Finish : State::Invalid;

  return data;
}
