#include <RfidDetector.h>
#include <HandlingUnit.h>
#include <MachineAPI.h>
#include <ChassisDigital.h>
#include <Arduino.h>

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
    targetValue = initialConfig;
    componentData = initialConfig;
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
    if (chassIsAtStartPosition() && handlingUnitIsAtStartPosition() &&
        chipDetected() && !recognizeChip() && millisOfLastSending + MILLIS_OF_LAST_SENDING <= millis())
    {
      debugLn("State 4: didnt recognize job.");
      millisOfLastSending = millis();

      Serial.print("{");
      Serial.print("\"" + String(DEST) + "\":\"?\",");
      Serial.print("\"" + String(HANDLING_UNIT) + "\":\"?\",");
      Serial.print("\"" + String(ID) + "\":[");
      for (int index = 0; index < RFID_LENGTH; index++)
      {
        Serial.print(String(rfidDetector->getData().id[index]));
        if (index + 1 != RFID_LENGTH)
        {
          Serial.print(",");
        }
        else
        {
          Serial.print("]");
        }
      }
      Serial.print("}");
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

  bool sortJobsAreEqual(SortJob sortJob1, SortJob sortJob2)
  {
    return !memcmp(sortJob1.id, sortJob2.id, RFID_LENGTH * sizeof(byte));
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
    printComponentStatus("HandlingUnit", handlingUnit->getState());
    printComponentStatus("RfidDetector", rfidDetector->getState());
    if (chipDetected())
    {
      debugLn("chipDetected");
    }
    if (recognizeChip())
    {
      debugLn("recognizeChip sortJob");
    }
    debugLn();
  }

  void printComponentStatus(String name, int state)
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
    default:
      debugLn(name + " undefined state.");
      break;
    }
  }
};

#endif