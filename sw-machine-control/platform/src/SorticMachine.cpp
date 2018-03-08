#include <Component.h>
#include <MachineAPI.h>
#include <SorticMachine.h>
#include <Placer.h>

#include <Debug.h>
#include <SPI.h>

Config SorticMachine::loop()
{
  chassis->executeOneStep();
  placer->executeOneStep();
  rfidDetector->executeOneStep();

  printStatus();

  if (chassIsAtStartPosition() && placerIsAtStartPosition())
  {
    chassis->off();

    if (allOff())
    {
      rfidDetector->on();
    }

    if (chipDetected())
    {
      placer->setAction(componentData.rfidSourcePosition);
      placer->on();
    }
  }

  if (placerHasChip())
  {
    chassis->setAction(componentData.rfids[getIndexOfRFidChip()].destination);
    chassis->on();
  }

  if (chassisReachedDestination())
  {
    placer->setAction(componentData.rfids[getIndexOfRFidChip()].placerPosition);
    placer->on();
  }

  if ((allFinished() || allOff()) && !placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    placer->setAction(componentData.placerSleepPosition);
    chassis->setAction(componentData.chassisStart);
    chassis->on();
    placer->on();
  }

  if (placerIsAtStartPosition() && !chassIsAtStartPosition())
  {
    chassis->on();
  }

  /*

 if ((allFinished() || allOff()) && !placerIsAtStartPosition())
  {
    placer->setAction(componentData.placerSleepPosition);
    placer->on();
  }
  if (allFinished() && placerIsAtStartPosition() && chassIsAtStartPosition())
  {
    chassis->off();
    placer->off();
    rfidDetector->off();
  }*/

  return componentData;
}

void SorticMachine::printComponentStatus(String name, State state)
{
  switch (state)
  {
  case State::On:
    debugLn(name + " is on.");
    break;
  case State::Invalid:
    debugLn(name + " is invalid.");
    break;
  case State::Off:
    debugLn(name + " is off.");
    break;
  case State::Finish:
    debugLn(name + " is finish.");
    break;
  }
}

void SorticMachine::printStatus()
{
  printComponentStatus("Chassis", chassis->getState());
  printComponentStatus("Placer", placer->getState());
  printComponentStatus("RfidDetector", rfidDetector->getState());
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
  return abs(componentData.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
}

bool SorticMachine::placerIsAtStartPosition()
{
  return componentData.placerSleepPosition == placer->getData();
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

int SorticMachine::getIndexOfRFidChip()
{
  for (int index = 0; index < componentData.rfidCount; index++)
  {
    SortJob id = componentData.rfids[index];

    if (!memcmp(id.id, rfidDetector->getData(), RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }
  return -1;
}
