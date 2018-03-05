#include <Actor.h>
#include <ConfigReciever.h>
#include <SorticMachine.h>
#include <Placer.h>

#include <Debug.h>
#include <SPI.h>

void SorticMachine::printStatus()
{
  debugLn("State:[On:1, Off:2, Invalid:4, Finish:8]:");
  debugLn("Chassis:" + String(chassis->getState()));
  debugLn("Placer:" + String(placer->getState()));
  debugLn("RfidDetector:" + String(rfidDetector->getState()));
  if (chassIsAtStartPosition())
  {
    debugLn("chassIsAtStartPosition");
  }
  if (placerIsAtStartPosition())
  {
    debugLn("placerIsAtStartPosition");
  }
  debugLn();
}

bool SorticMachine::chassIsAtStartPosition()
{
  return abs(sensorData.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
}

bool SorticMachine::placerIsAtStartPosition()
{
  return sensorData.placerSleepPosition == placer->getData();
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
  chassis->execute();
  placer->execute();
  rfidDetector->execute();

  printStatus();

  if (chassIsAtStartPosition() && placerIsAtStartPosition())
  {
    if (allOff())
    {
      rfidDetector->on();
    }

    if (chipDetected())
    {
      placer->setAction(sensorData.rfidSourcePosition);
      placer->on();
    }
  }

  if (placerHasChip())
  {
    chassis->setAction(sensorData.rfids[getIndexOfRFidChip()].destination);
    chassis->on();
  }

  if (chassisReachedDestination())
  {
    placer->setAction(sensorData.rfids[getIndexOfRFidChip()].placerPosition);
    placer->on();
  }

  if ((allFinished() || allOff()) && !placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    placer->setAction(sensorData.placerSleepPosition);
    chassis->setAction(sensorData.chassisStart);

    placer->on();
  }

  if ((allFinished() || allOff()) && placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    chassis->on();
  }
  /*
  
 if ((allFinished() || allOff()) && !placerIsAtStartPosition())
  {
    placer->setAction(sensorData.placerSleepPosition);
    placer->on();
  }
  if (allFinished() && placerIsAtStartPosition() && chassIsAtStartPosition())
  {
    chassis->off();
    placer->off();
    rfidDetector->off();
  }*/

  return sensorData;
}

int SorticMachine::getIndexOfRFidChip()
{
  for (int index = 0; index < sensorData.rfidCount; index++)
  {
    RFidChip id = sensorData.rfids[index];

    if (!memcmp(id.id, rfidDetector->getData(), RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }
  return -1;
}