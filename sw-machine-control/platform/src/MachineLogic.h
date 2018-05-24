#ifndef MachineLogic_h
#define MachineLogic_h

#include <RfidDetector.h>
#include <HandlingUnit.h>
#include <Receiver.h>
#include <ChassisDigital.h>
#include <Arduino.h>

#include <Component.h>
#include <Debug.h>

class MachineLogic : public Component<MachineAPI>
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
  MachineAPI loop()
  {
    if (targetValue.getState && millisOfLastSendingGetState + MILLIS_OF_LAST_SENDING <= millis())
    {
      millisOfLastSendingGetState = millis();
      Serial.print("{\"method\":\"getState\", \"params\":{");
      Serial.print("\"chassis\":"+ String((int)chassis->getState())+",");
      Serial.print("\"rfidDetector\":"+ String((int)rfidDetector->getState()) + ",");
      Serial.print("\"handlingUnit\":" + String((int)handlingUnit->getState()));
      Serial.println("}}");
    }

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
      Serial.print("{\"method\":\"askForSortJob\", \"params\":{");
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
      Serial.println("}}");
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

    if(targetValue.powerOn)
    {
      chassis->executeOneStep();
      handlingUnit->executeOneStep();
      rfidDetector->executeOneStep();
    }

    return targetValue;
  }

private:
  Component<HandlingUnitPosition> *handlingUnit;
  Component<SortJob> *rfidDetector;
  Component<int> *chassis;
  unsigned long millisOfLastSending{0};
  unsigned long millisOfLastSendingGetState{0};

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
};

#endif
