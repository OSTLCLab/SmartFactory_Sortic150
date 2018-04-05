#include <Chassis.h>
#include <RfidDetector.h>
#include <HandlingUnit.h>
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
  MachineLogic(Component<HandlingUnitPosition> *handlingUnit,
               Component<int> *rfidDetector,
               Component<int> *chassis) : handlingUnit{handlingUnit},
                                          chassis{chassis},
                                          rfidDetector{rfidDetector}
  {
  }

protected:
  Config loop()
  {
    if ((allFinished() || allOff()) && !handlingUnitIsAtStartPosition() && !chassIsAtStartPosition() && !chassisReachedDestination())
    {
      debugLn("State 1: Put HandlingUnit in its initposition.");
      handlingUnit->setAction(targetValue.handlingUnitInitPosition);
      handlingUnit->on();
    }

    if (handlingUnitIsAtStartPosition() && !chassIsAtStartPosition())
    {
      debugLn("State 2: Put Chassis in its initposition.");
      chassis->setAction(targetValue.chassisStart);
      chassis->on();
    }

    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() && !chipDetected())
    {
      debugLn("State 3: Check for new sortjobs.");
      chassis->wait();
      handlingUnit->wait();
      rfidDetector->on();
    }

    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() && chipDetected())
    {
      debugLn("State 4: Get the new sortjob.");
      handlingUnit->setAction(targetValue.rfidSourcePosition);
      handlingUnit->on();
    }

    if (handlingUnitHasChip())
    {
      debugLn("State 5: Put chassis at the sortjob position.");
      chassis->setAction(targetValue.rfids[rfidDetector->getData()].destination);
      chassis->on();
    }

    if (!allOff() && chassisReachedDestination() && !handlingUnitDeposeChip())
    {
      debugLn("State 6: Put handlingUnit at the sortjob position.");
      handlingUnit->setAction(targetValue.rfids[rfidDetector->getData()].handlingUnitPosition);
      handlingUnit->on();
    }

    if (!targetValue.powerOn || (chassisReachedDestination() && handlingUnitDeposeChip()))
    {
      debugLn("State 7: Go to State 1.");
      handlingUnit->wait();
      chassis->wait();
      rfidDetector->wait();
    }

    chassis->executeOneStep();
    handlingUnit->executeOneStep();
    rfidDetector->executeOneStep();

    return targetValue;
  }

private:
  Component<HandlingUnitPosition> *handlingUnit;
  Component<int> *chassis;
  Component<int> *rfidDetector;

  bool chassIsAtStartPosition()
  {
    return chassis->getState() != Running && abs(targetValue.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
  }

  bool handlingUnitDeposeChip()
  {
    return chipDetected() && targetValue.rfids[rfidDetector->getData()].handlingUnitPosition == handlingUnit->getData();
  }

  bool handlingUnitIsAtStartPosition()
  {
    return targetValue.handlingUnitInitPosition == handlingUnit->getData();
  }
  bool chipDetected()
  {
    return rfidDetector->getState() == Finish && rfidDetector->getData() != -1;
  }

  bool allOff()
  {
    return chassis->getState() == Waiting &&
           handlingUnit->getState() == Waiting &&
           rfidDetector->getState() == Waiting;
  }

  bool handlingUnitHasChip()
  {
    return handlingUnit->getState() == Finish &&
           chipDetected() &&
           chassIsAtStartPosition() &&
           !handlingUnitIsAtStartPosition();
  }

  bool chassisReachedDestination()
  {
    return chassis->getState() == Finish &&
           chipDetected() &&
           (abs(targetValue.rfids[rfidDetector->getData()].destination - chassis->getData()) <= CHASSIS_TOLERANCE);
  }

  bool allFinished()
  {
    return chassis->getState() == Finish &&
           handlingUnit->getState() == Finish &&
           rfidDetector->getState() == Finish;
  }

  void printStatus()
  {
    printComponentStatus("Chassis", chassis->getState());
    printComponentStatus("handlingUnit", handlingUnit->getState());
    printComponentStatus("RfidDetector", rfidDetector->getState());
    if (chassIsAtStartPosition())
    {
      debugLn("chassIsAtStartPosition");
    }
    if (handlingUnitHasChip())
    {
      debugLn("handlingUnitHasChip");
    }
    if (handlingUnitIsAtStartPosition())
    {
      debugLn("handlingUnitIsAtStartPosition");
    }
    if (chipDetected())
    {
      debugLn("chipDetected" + String(rfidDetector->getData()));
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