#include <Actor.h>
#include <ConfigReciever.h>
#include <SorticMachine.h>
#include <Placer.h>

#include <Arduino.h>
#include <SPI.h>

void SorticMachine::printStatus()
{
  Serial.println();
  Serial.println("State:[On:1, Off:2, Invalid:4, Finish:8]:");
  Serial.println("Chassis:" + String(chassis->getState()));
  Serial.println("Placer:" + String(placer->getState()));
  Serial.println("RfidDetector:" + String(rfidDetector->getState()));
  if (chassIsAtStartPosition())
  {
    Serial.println("chassIsAtStartPosition");
  }
  if (placerIsAtStartPosition())
  {
    Serial.println("placerIsAtStartPosition");
  }
  Serial.println();
}

bool SorticMachine::chassIsAtStartPosition()
{
  return data.startPosition == chassis->getData();
}

bool SorticMachine::placerIsAtStartPosition()
{
  return data.placerStartPosition == placer->getData();
}

bool SorticMachine::chipDetected()
{
  return getIndexOfRFidChip() != -1;
}

bool SorticMachine::allOff()
{
  return chassis->getState() == State::Off &&
         placer->getState() == State::Off &&
         rfidDetector->getState() == State::Off;
}

bool SorticMachine::placerHasChip()
{
  return placer->getState() == State::Finish &&
         chipDetected() && chassIsAtStartPosition() &&
         !placerIsAtStartPosition();
}

bool SorticMachine::chassisReachedDestination()
{
  return chassis->getState() == State::Finish &&
         chipDetected() && !chassIsAtStartPosition();
}

bool SorticMachine::allFinished()
{
  return chassis->getState() == State::Finish &&
         placer->getState() == State::Finish &&
         rfidDetector->getState() == State::Finish;
}

Config SorticMachine::loop()
{
  printStatus();

  if (chassIsAtStartPosition() && placerIsAtStartPosition())
  {
    if (allOff())
    {
      rfidDetector->on();
    }

    if (chipDetected())
    {
      placer->setAction(data.chipPosition);
      placer->on();
    }
  }

  if (placerHasChip())
  {
    chassis->setAction(data.rfidChips[getIndexOfRFidChip()].targetPosition);
    chassis->on();
  }

  if (chassisReachedDestination())
  {
    placer->setAction(data.rfidChips[getIndexOfRFidChip()].placerPosition);
    placer->on();
  }

  if ((allFinished() || allOff()) && !placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    placer->setAction(data.placerStartPosition);
    placer->on();
  }
  /*
  if ((allFinished() || allOff()) && !placerIsAtStartPosition())
  {
    placer->setAction(data.placerStartPosition);
    placer->on();
  }

  if ((allFinished() || allOff()) && placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    chassis->setAction(data.startPosition);
    chassis->on();
  }

  if (allFinished() && placerIsAtStartPosition() && chassIsAtStartPosition())
  {
    chassis->off();
    placer->off();
    rfidDetector->off();
  }*/

  return data;
}

int SorticMachine::getIndexOfRFidChip()
{
  for (int index = 0; index < data.sizeOfRfidChips; index++)
  {
    RFidChip rfidChip = data.rfidChips[index];

    if (!memcmp(rfidChip.rfidChip, rfidDetector->getData(), RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }
  return -1;
}