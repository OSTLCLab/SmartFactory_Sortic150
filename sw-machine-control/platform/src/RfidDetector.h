#pragma once

#include <MFRC522.h>
#include <Sensor.h>
#include <Config.h>

class RfidDetector : public Sensor
{
public:
  RfidDetector(MFRC522 *rfidReader) : rfidReader{rfidReader} {}

  Print &get(Print &out)
  {
    if (cardPresent())
    {
      for (auto index = 0; index < RFID_LENGTH; index++)
      {
        out << rfidReader->uid.uidByte[index];
      }
    }

    return out;
  }

private:
  MFRC522 *rfidReader;
  bool cardPresent()
  {
    return rfidReader->PICC_IsNewCardPresent() &&
           rfidReader->PICC_ReadCardSerial() &&
           rfidReader->uid.size != 0;
  }
};