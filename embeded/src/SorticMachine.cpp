#include <Actor.h>
#include <ConfigReciever.h>
#include <SorticMachine.h>
#include <Placer.h>

#include <Arduino.h>
#include <SPI.h>

Config SorticMachine::loop()
{

  int chassisPosition = chassis->getData();
  byte *rfidNr = rfidDetector->getData();
  PlacerPosition placerPosition = placer->getData();

  Serial.println();
  Serial.println("State:[On:1, Off:2, Invalid:4, Finish:8]:");
  Serial.println("Chassis:" + String(chassis->getState()));
  Serial.println("Placer:" + String(placer->getState()));
  Serial.println("RfidDetector:" + String(rfidDetector->getState()));
  Serial.println();

  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off)
  {
    chassis->setAction(data.startPosition);
    chassis->on();
  }
  /*
  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off &&
      chassisPosition != data.startPosition)
  {
    chassis->setAction(data.startPosition);
    chassis->on();
  }
  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off &&
      chassisPosition == data.startPosition &&
      placerPosition != data.placerStartPosition)
  {
    placer->setAction(data.placerStartPosition);
    placer->on();
  }
 
  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off &&
      chassisPosition == data.startPosition &&
      placerPosition == data.placerStartPosition)
  {
    placer->off();
    chassis->off();
    rfidDetector->on();
  }*/

  return data;
}

int SorticMachine::getIndexOfRFidChip(Config config, byte detectedRfid[8])
{
  for (int index = 0; index < config.sizeOfRfidChips; index++)
  {
    RFidChip rfidChip = config.rfidChips[index];

    if (!memcmp(rfidChip.rfidChip, detectedRfid, RFID_LENGTH * sizeof(byte)))
    {
      Serial.println("Found RFID");
      return index;
    }
  }
  return -1;
}