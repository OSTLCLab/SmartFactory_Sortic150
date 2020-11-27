#pragma once

#include <MFRC522.h>
#include <Sensor.h>

class RfidDetector : public Sensor
{
public:
  RfidDetector(MFRC522 *rfidReader) : rfidReader{rfidReader} {}

  Stream &get(Stream &obj)
  {
    if (rfidReader->PICC_IsNewCardPresent() && rfidReader->PICC_ReadCardSerial() && rfidReader->uid.size != 0)
    {
      for (auto index = 0; index < RFID_LENGTH; index++)
      {
        obj << rfidReader->uid.uidByte[index];
      }
    }

    return obj;
  }

private:
  MFRC522 *rfidReader;
};
