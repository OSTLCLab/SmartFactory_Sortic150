#include <Chassis.h>
#include <RfidDetector.h>
#include <Placer.h>
#include <MachineAPI.h>

#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <Wire.h>
#include <Debug.h>

#include <Adafruit_MotorShield.h>

#ifndef MachineLogic_h
#define MachineLogic_h

class MachineLogic : public Component<Config>
{
public:
  MachineLogic(Component<PlacerPosition> *placer,
               Component<byte *> *rfidDetector,
               Component<int> *chassis,
               Component<Config> *machineAPI) : placer{placer},
                                                chassis{chassis},
                                                rfidDetector{rfidDetector},
                                                machineAPI{machineAPI}
  {
    this->componentData = machineAPI->getData();
  }

protected:
  Config loop()
  {
    printStatus();
    chassis->executeOneStep();
    placer->executeOneStep();
    rfidDetector->executeOneStep();

    if (chassIsAtStartPosition() && placerIsAtStartPosition())
    {
      if (chipDetected())
      {
        placer->setAction(componentData.rfidSourcePosition);
        placer->on();
      }
      else
      {
        chassis->wait();
        placer->wait();
        rfidDetector->on();
      }
    }

    if (placerHasChip())
    {
      chassis->setAction(componentData.rfids[chipIndex].destination);
      chassis->on();
    }

    if (chassisReachedDestination())
    {
      placer->setAction(componentData.rfids[chipIndex].placerPosition);
      placer->on();
    }

    if ((allFinished() || allOff()) && !placerIsAtStartPosition() && !chassIsAtStartPosition())
    {
      placer->setAction(componentData.placerSleepPosition);
      placer->on();
    }

    if (placerIsAtStartPosition() && !chassIsAtStartPosition())
    {
      chassis->setAction(componentData.chassisStart);
      chassis->on();
    }

    if ((allFinished() || allOff()) && !placerIsAtStartPosition())
    {
      placer->setAction(componentData.placerSleepPosition);
      placer->on();
    }
    if (allFinished() && placerIsAtStartPosition() && chassIsAtStartPosition())
    {
      chassis->wait();
      placer->wait();
      rfidDetector->wait();
    }

    return componentData;
  }

private:
  Component<PlacerPosition> *placer;
  Component<int> *chassis;
  Component<byte *> *rfidDetector;
  Component<Config> *machineAPI;
  int chipIndex{-1};

  bool chassIsAtStartPosition()
  {
    return abs(componentData.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
  }

  bool placerIsAtStartPosition()
  {
    return componentData.placerSleepPosition == placer->getData();
  }

  bool chipDetected()
  {
    if (chipIndex != -1)
    {
      return true;
    }
    chipIndex = getIndexOfRFidChip();
    return chipIndex != -1;
  }

  bool allOff()
  {
    return chassis->getState() == Waiting &&
           placer->getState() == Waiting &&
           rfidDetector->getState() == Waiting;
  }

  bool placerHasChip()
  {
    return placer->getState() == Finish &&
           chipDetected() &&
           chassIsAtStartPosition() &&
           !placerIsAtStartPosition();
  }

  bool chassisReachedDestination()
  {
    return chassis->getState() == Finish &&
           chipDetected() &&
           !chassIsAtStartPosition();
  }

  bool allFinished()
  {
    return chassis->getState() == Finish &&
           placer->getState() == Finish &&
           rfidDetector->getState() == Finish;
  }

  int getIndexOfRFidChip()
  {
    for (int index = 0; index < componentData.rfidCount; index++)
    {
      SortJob sortJob = componentData.rfids[index];
      bool areEqual = true;
      for (int index2 = 0; index2 < 8; index2++)
      {
        if (!areEqual || rfidDetector->getData()[index2] != sortJob.id[index2])
        {
          areEqual = false;
        }
      }

      if (areEqual)
      {
        debugLn(index);
        return index;
      }
    }
    return -1;
  }
  void printStatus()
  {
    printComponentStatus("Chassis", chassis->getState());
    printComponentStatus("Placer", placer->getState());
    printComponentStatus("RfidDetector", rfidDetector->getState());
    if (chassIsAtStartPosition())
    {
      debugLn("chassIsAtStartPosition");
    }
    if (placerHasChip())
    {
      debugLn("placerHasChip");
    }
    if (placerIsAtStartPosition())
    {
      debugLn("placerIsAtStartPosition");
    }
    if (chipDetected())
    {
      debugLn("chipDetected");
    }
    debugLn();
  }

  void printComponentStatus(String name, State state)
  {
    switch (state)
    {
    case Running:
      debugLn(name + " is running.");
      break;
    case Invalid:
      debugLn(name + " is invalid.");
      break;
    case Waiting:
      debugLn(name + " is waiting.");
      break;
    case Finish:
      debugLn(name + " is finish.");
      break;
    }
  }
};

#endif