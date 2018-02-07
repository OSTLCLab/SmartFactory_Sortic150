#include "RailMover.h"

#include "Arduino.h"
#include "SorticFramework.h"

RailMover::RailMover(Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC) : Mover()
{
  positionValues[0] = PosPickup;
  positionValues[1] = posDropA;
  positionValues[2] = posDropB;
  positionValues[3] = posDropC;
  DriverMotor = tempDriverMotor;
  distanceSensorPin = tempDistanceSensorPin;
  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter = MedianFilter(rawSensorValue);

  DriverMotor->run(FORWARD); //FORWARD = Nach Rechts, BACKWARD = Nach Links
  DriverMotor->run(RELEASE);
  DriverMotor->setSpeed(0);
}

void RailMover::moveToPosition(MoverPosition newTarget)
{
  bahnHasStopped = false;
  targetPosition = getPositionValue(newTarget); //ToDo: Create correct target location
}

int RailMover::getPositionValue(MoverPosition tempPosition)
{
  for (int i = 0; i < 4; i++)
  {
    if (positionMarked[i] == tempPosition)
    {
      return positionValues[i];
    }
  }
}

bool RailMover::moverLoop()
{ //true = complete, false = in progress
  //Maximum sensor value ~= 565, use max sensor value if larger
  if (targetPosition > positionMax)
  {
    targetPosition = positionMax;
  }

  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter.UpdateFilter(rawSensorValue);
  filteredSensorValue = thisMedianFilter.getFilterValue();
  //Serial.println(filteredSensorValue);
  if (bahnHasStopped == true)
  {
    DriverMotor->run(RELEASE);
  }

  if ((bahnHasStopped == false) and (filteredSensorValue != 0))
  {
    //Funktion fuer Uebertragung in nutzbare grösse
    int currentPosition = filteredSensorValue;
    //Serial.println(currentPosition);

    //In richtige Richtung fahren
    if (currentPosition > targetPosition)
    {
      //Drive left
      if (ForwardIsLeft)
      {
        DriverMotor->run(FORWARD);
      }
      else
      {
        DriverMotor->run(BACKWARD);
      }
    }
    else
    {
      //Drive right
      if (ForwardIsLeft)
      {
        DriverMotor->run(BACKWARD);
      }
      else
      {
        DriverMotor->run(FORWARD);
      }
    }

    distanceToTarget = abs(currentPosition - targetPosition);

    //Set Speed relative to target
    if (distanceToTarget > 150)
    {
      DriverMotor->setSpeed(maxSpeed);
    }
    else
    {
      DriverMotor->setSpeed(distanceToTarget + 70);
    }

    //Stop at target
    //driveTollerance = distanceToTarget/200+1; //100->2, 200->3, 300->4, 400->5, 500->6
    if (distanceToTarget < driveTollerance)
    {
      //Serial.println(abs(currentPosition-targetPosition));
      DriverMotor->setSpeed(0);
      bahnHasStopped = true;
    }
  }

  return bahnHasStopped;
}
