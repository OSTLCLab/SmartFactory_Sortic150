#include <Actor.h>
#include <ConfigReciever.h>
#include <SorticMachine.h>
#include <PlacerPerformance.h>

#include <Arduino.h>
#include <SPI.h>

Config SorticMachine::loop()
{
  placer->getData();
  int chassisPosition = chassis->getData();
  byte *rfidNr = rfidDetector->getData();

  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off)
  {
    Serial.println("init State");
    chassis->setAction(data.startPosition);
    chassis->on();
  }

  if (chassis->getState() == State::Finish &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off)
  {
    Serial.println("Handle Chip");
    int index = this->getIndexOfRFidChip(data, rfidNr);
    PlacerPosition position = {};
    if (index == -1)
    {
      Serial.println("Get Chip");
      placer->setAction(position);
    }
    else
    {
      Serial.println("Store Chip");
      position.direction = data.rfidChips[index].targetDirection;
      placer->setAction(position);
    }

    chassis->off();
    placer->on();
  }

  if (chassis->getState() == State::Off &&
      placer->getState() == State::Finish &&
      rfidDetector->getState() == State::Off && chassisPosition == data.startPosition)
  {
    Serial.println("Scan RFID at Pos0");
    rfidDetector->on();
    placer->off();
  }
  else
  {
    placer->off();
  }

  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Finish)
  {
    Serial.println("Go2PosX");
    chassis->setAction(this->getIndexOfRFidChip(data, rfidNr));
    chassis->on();
    rfidDetector->off();
  }

  return data;
}

int SorticMachine::getIndexOfRFidChip(Config config, byte detectedRfid[8])
{
  for (int index = 0; index < config.sizeOfRfidChips; index++)
  {
    RFidChip rfidChip = config.rfidChips[index];

    //foundrfid
    if (!memcmp(rfidChip.rfidChip, detectedRfid, RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }
  return -1;
}