#include <Actor.h>
#include <ConfigReciever.h>
#include <SorticMachine.h>
#include <Placer.h>

#include <Arduino.h>
#include <SPI.h>

Config SorticMachine::loop()
{
  PlacerPosition placerPosition = placer->getData();
  placer->setAction(PlacerPosition::Front);
  /* int chassisPosition = chassis->getData();
  byte *rfidNr = rfidDetector->getData();
  int index = -1;
  if (chassis->getState() == State::Finish)
  {
    index = this->getIndexOfRFidChip(data, rfidNr);
  }

  Serial.println();
  Serial.println("State:[On:1, Off:2, Invalid:4, Finish:8]:");
  delay(50);
  Serial.println("Chassis:" + String(chassis->getState()));
  delay(50);
  Serial.println("Placer:" + String(placer->getState()));
  delay(50);
  Serial.println("RfidDetector:" + String(rfidDetector->getState()));
  delay(50);
  Serial.println();

  placer->on();*/

  /*if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off)
  {
    Serial.println("init State");
    chassis->setAction(data.startPosition);
    chassis->on();
  }

  if (chassis->getState() == State::Finish &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off && (index < 0 || data.rfidChips[index].targetPosition != chassisPosition))
  {
    Serial.println("...");
    PlacerPosition position = {};
    placer->setAction(position);
    placer->on();
    chassis->off();
  }

  if (chassis->getState() == State::Off &&
      placer->getState() == State::Finish &&
      rfidDetector->getState() == State::Off)
  {
    placer->off();
    rfidDetector->on();
  }

  //RFIDgescannt
  if (chassis->getState() == State::Off &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Finish)
  {
    rfidDetector->off();

    //Warte bis nächste gültige bzw. status ist nocht zu definieren
    if (index == -1)
    {
      rfidDetector->on();
    }
    else
    {
      chassis->on();
      chassis->setAction(data.rfidChips[index].targetPosition);
    }
  }
  //todo bei position muss filter beruecksichtigt werden.
  if (chassis->getState() == State::Finish &&
      placer->getState() == State::Off &&
      rfidDetector->getState() == State::Off && index >= 0 && data.rfidChips[index].targetPosition == chassisPosition)
  {
    Serial.println("...");
    chassis->off();
    PlacerPosition position = {};
    position.direction = data.rfidChips[index].targetDirection;
    placer->setAction(position);
    placer->on();
  }

  //todo bei position muss filter beruecksichtigt werden.
  if (chassis->getState() == State::Off &&
      placer->getState() == State::Finish &&
      rfidDetector->getState() == State::Off && index >= 0 && data.rfidChips[index].targetPosition == chassisPosition)
  {
    chassis->setAction(data.startPosition);
    chassis->on();
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