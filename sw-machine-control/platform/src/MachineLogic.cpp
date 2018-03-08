#include <Component.h>
#include <MachineAPI.h>
#include <MachineLogic.h>
#include <Placer.h>

#include <Debug.h>
#include <SPI.h>

Config MachineLogic::loop()
{
  chassis->executeOneStep();
  placer->executeOneStep();
  rfidDetector->executeOneStep();

  printStatus();

  if (chassIsAtStartPosition() && placerIsAtStartPosition())
  {
    if (chipDetected())
    {
      placer->setAction(componentData.rfidSourcePosition);
      placer->on();
    }
    else
    {
      chassis->off();
      placer->off();
      rfidDetector->on();
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

void MachineLogic::printComponentStatus(String name, State state)
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

void MachineLogic::printStatus()
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

bool MachineLogic::chassIsAtStartPosition()
{
  return abs(componentData.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
}

bool MachineLogic::placerIsAtStartPosition()
{
  return componentData.placerSleepPosition == placer->getData();
}

bool MachineLogic::chipDetected()
{
  return getIndexOfRFidChip() != -1;
}

bool MachineLogic::allOff()
{
  return chassis->getState() == State::Off &&
         placer->getState() == State::Off &&
         rfidDetector->getState() == State::Off;
}

bool MachineLogic::placerHasChip()
{
  return placer->getState() == State::Finish &&
         chipDetected() && chassIsAtStartPosition() &&
         !placerIsAtStartPosition();
}

bool MachineLogic::chassisReachedDestination()
{
  return chassis->getState() == State::Finish &&
         chipDetected() && !chassIsAtStartPosition();
}

bool MachineLogic::allFinished()
{
  return chassis->getState() == State::Finish &&
         placer->getState() == State::Finish &&
         rfidDetector->getState() == State::Finish;
}

int MachineLogic::getIndexOfRFidChip()
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
