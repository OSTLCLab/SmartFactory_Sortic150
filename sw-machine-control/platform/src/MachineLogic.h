#include <ArduinoJson.h>
#include <Chassis.h>
#include <RfidDetector.h>
#include <HandlingUnit.h>
#include <MachineAPI.h>

#include <Component.h>
#include <Debug.h>

#ifndef MachineLogic_h
#define MachineLogic_h

class MachineLogic : public Component<Config>
{
public:
  MachineLogic(Component<HandlingUnitPosition> *handlingUnit,
               Component<SortJob> *rfidDetector,
               Component<int> *chassis) : handlingUnit{handlingUnit},
                                          rfidDetector{rfidDetector},
                                          chassis{chassis}
  {
  }

protected:
  Config loop()
  {
    if ((allFinished() || allOff()) && !handlingUnitIsAtStartPosition() && !chassIsAtStartPosition() && !chassisReachedDestination())
    {
      debugLn("State 1: Put HandlingUnit in its initposition.");
      handlingUnit->setAction(targetValue.handlingUnitStartPosition);
    }

    if (handlingUnitIsAtStartPosition() && !chassIsAtStartPosition())
    {
      debugLn("State 2: Put Chassis in its initposition.");
      chassis->setAction(targetValue.chassisStart);
    }

    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() && !chipDetected())
    {
      debugLn("State 3: Check for new sortjobs.");
      chassis->wait();
      handlingUnit->wait();
      rfidDetector->on();
    }
    unsigned long actualMillis = millis();

    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() &&
        chipDetected() && !recognizeChip() && millisOfLastSending + MILLIS_OF_LAST_SENDING <= actualMillis)
    {
      debugLn("State 4: didnt recognize job.");
      millisOfLastSending = millis();
      StaticJsonBuffer<200> buffer;
      JsonObject &root = buffer.createObject();
      root[DEST] = "?";
      root[HANDLING_UNIT] = "?";
      JsonArray &arr = root.createNestedArray(ID);
      arr.copyFrom(rfidDetector->getData().id, RFID_LENGTH);
      root.printTo(Serial);
    }

    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() && recognizeChip())
    {
      debugLn("State 5: Get the new sortjob.");
      handlingUnit->setAction(targetValue.sortJobSourcePosition);
    }

    if (handlingUnitHasChip())
    {
      debugLn("State 6: Put chassis at the sortjob position.");
      chassis->setAction(targetValue.sortJob.destination);
    }

    if (!allOff() && chassisReachedDestination() && !handlingUnitDeposeChip())
    {
      debugLn("State 7: Put handlingUnit at the sortjob position.");
      handlingUnit->setAction(targetValue.sortJob.handlingUnitPosition);
    }

    if (!targetValue.powerOn || (chassisReachedDestination() && handlingUnitDeposeChip()))
    {
      debugLn("State 8: Go to State 1.");
      targetValue.sortJob = DEFAULT_SORTJOB;
      handlingUnit->wait();
      chassis->wait();
      rfidDetector->wait();
    }
    printStatus();

    chassis->executeOneStep();
    handlingUnit->executeOneStep();
    rfidDetector->executeOneStep();

    return targetValue;
  }

private:
  Component<HandlingUnitPosition> *handlingUnit;
  Component<SortJob> *rfidDetector;
  Component<int> *chassis;
  unsigned long millisOfLastSending{0};

  bool chassIsAtStartPosition()
  {
    return chassis->getState() != Running && abs(targetValue.chassisStart - chassis->getData()) <= CHASSIS_TOLERANCE;
  }

  bool handlingUnitDeposeChip()
  {
    return recognizeChip() && targetValue.sortJob.handlingUnitPosition == handlingUnit->getData();
  }

  bool handlingUnitIsAtStartPosition()
  {
    return targetValue.handlingUnitStartPosition == handlingUnit->getData();
  }

  bool chipDetected()
  {
    return rfidDetector->getState() == Finish;
  }

  bool recognizeChip()
  {
    return chipDetected() && targetValue.sortJob.destination != -1 &&
           targetValue.sortJob.handlingUnitPosition != NoPosition &&
           (sortJobsAreEqual(targetValue.sortJob, rfidDetector->getData()) ||
            sortJobsAreEqual(targetValue.sortJob, DEFAULT_SORTJOB));
  }

  bool sortJobsAreEqual(SortJob sortJob1, SortJob sortjob2)
  {
    return !memcmp(sortJob1.id, sortjob2.id, RFID_LENGTH * sizeof(byte));
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
           recognizeChip() &&
           chassIsAtStartPosition() &&
           !handlingUnitIsAtStartPosition();
  }

  bool chassisReachedDestination()
  {
    return chassis->getState() == Finish &&
           recognizeChip() &&
           (abs(targetValue.sortJob.destination - chassis->getData()) <= CHASSIS_TOLERANCE);
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