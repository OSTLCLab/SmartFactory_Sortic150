#include <Chassis.h>
#include <RfidDetector.h>
#include <GrippController.h>
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
  MachineLogic(Component<GrippPosition> *grippController,
               Component<int> *rfidDetector,
               Component<int> *chassis,
               Component<Config> *machineAPI) : grippController{grippController},
                                                chassis{chassis},
                                                rfidDetector{rfidDetector},
                                                machineAPI{machineAPI}
  {
    this->componentData = machineAPI->getData();
  }

protected:
  Config loop()
  {
    if (chassIsAtStartPosition() && grippControllerIsAtStartPosition())
    {
      if (chipDetected())
      {
        debugLn("1");
        grippController->setAction(componentData.rfidSourcePosition);
        grippController->on();
      }
      else
      {
        debugLn("2");
        chassis->wait();
        grippController->wait();
        rfidDetector->on();
      }
    }

    if (grippControllerHasChip())
    {
      debugLn("3");
      chassis->setAction(componentData.rfids[rfidDetector->getData()].destination);
      chassis->on();
    }

    if (!allOff() && chassisReachedDestination() && !grippControllerDeposeChip())
    {
      debugLn("4");
      grippController->setAction(componentData.rfids[rfidDetector->getData()].GrippPosition);
      grippController->on();
    }

    if ((allFinished() || allOff()) && !grippControllerIsAtStartPosition() && !chassIsAtStartPosition() && !chassisReachedDestination())
    {
      debugLn("5");
      grippController->setAction(componentData.grippSleepPosition);
      grippController->on();
    }

    if (grippControllerIsAtStartPosition() && !chassIsAtStartPosition())
    {
      debugLn("6");
      chassis->setAction(componentData.chassisStart);
      chassis->on();
    }

    if (chassisReachedDestination() && grippControllerDeposeChip())
    {
      debugLn("7");
      grippController->wait();
      chassis->wait();
      rfidDetector->wait();
    }

    printStatus();
    chassis->executeOneStep();
    grippController->executeOneStep();
    rfidDetector->executeOneStep();

    return componentData;
  }

private:
  Component<GrippPosition> *grippController;
  Component<int> *chassis;
  Component<int> *rfidDetector;
  Component<Config> *machineAPI;

  bool chassIsAtStartPosition()
  {
    return abs(componentData.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
  }

  bool grippControllerDeposeChip()
  {
    return chipDetected() && componentData.rfids[rfidDetector->getData()].GrippPosition == grippController->getData();
  }

  bool grippControllerIsAtStartPosition()
  {
    return componentData.grippSleepPosition == grippController->getData();
  }

  bool chipDetected()
  {
    return rfidDetector->getState() != Waiting && rfidDetector->getData() != -1;
  }

  bool allOff()
  {
    return chassis->getState() == Waiting &&
           grippController->getState() == Waiting &&
           rfidDetector->getState() == Waiting;
  }

  bool grippControllerHasChip()
  {
    return grippController->getState() == Finish &&
           chipDetected() &&
           chassIsAtStartPosition() &&
           !grippControllerIsAtStartPosition();
  }

  bool chassisReachedDestination()
  {
    return chassis->getState() == Finish &&
           chipDetected() &&
           (abs(componentData.rfids[rfidDetector->getData()].destination - chassis->getData()) <= CHASSIS_TOLERANCE);
  }

  bool allFinished()
  {
    return chassis->getState() == Finish &&
           grippController->getState() == Finish &&
           rfidDetector->getState() == Finish;
  }

  void printStatus()
  {
    printComponentStatus("Chassis", chassis->getState());
    printComponentStatus("grippController", grippController->getState());
    printComponentStatus("RfidDetector", rfidDetector->getState());
    if (chassIsAtStartPosition())
    {
      debugLn("chassIsAtStartPosition");
    }
    if (grippControllerHasChip())
    {
      debugLn("grippControllerHasChip");
    }
    if (grippControllerIsAtStartPosition())
    {
      debugLn("grippControllerIsAtStartPosition");
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