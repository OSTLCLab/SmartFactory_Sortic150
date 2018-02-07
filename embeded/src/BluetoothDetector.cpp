#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#include "BluetoothDetector.h"

BluetoothDetector::BluetoothDetector(MFRC522 *bluetoothDetector) : Detector()
{
  mfrc522 = *bluetoothDetector;
  mfrc522.PCD_Init();
}

bool BluetoothDetector::RfidCardIsPresent()
{
  return mfrc522.PICC_IsNewCardPresent();
}

void BluetoothDetector::getPartArray(byte partArray[])
{
  //Example: partArray[0] = 1;
  byte blockAddr = 0;
  byte bufferSize = 18;
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, partArray, &bufferSize);

  // Error handling:
  if (status != MFRC522::STATUS_OK)
  {
    if (status == MFRC522::STATUS_COLLISION)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 1;
      }
    }
    else if (status == MFRC522::STATUS_CRC_WRONG)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 2;
      }
    }
    else if (status == MFRC522::STATUS_ERROR)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 3;
      }
    }
    else if (status == MFRC522::STATUS_INTERNAL_ERROR)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 4;
      }
    }
    else if (status == MFRC522::STATUS_INVALID)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 5;
      }
    }
    else if (status == MFRC522::STATUS_MIFARE_NACK)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 6;
      }
    }
    else if (status == MFRC522::STATUS_NO_ROOM)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 7;
      }
    }
    else if (status == MFRC522::STATUS_TIMEOUT)
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 8;
      }
    }
    else
    {
      for (int i = 0; i < 8; i++)
      {
        partArray[i] = 0;
      }
    }
  }
}
