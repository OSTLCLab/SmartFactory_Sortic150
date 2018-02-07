#include "Chassis.h"

#include "Arduino.h"
#include "SorticFramework.h"

Chassis::Chassis(Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC)
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
  moveToPosition(ChassisPosition::pickUp);
}

void Chassis::moveToPosition(ChassisPosition newTarget)
{
  state.hasStopped = false;
  targetPosition = getPositionValue(newTarget); //ToDo: Create correct target location
}

int Chassis::getPositionValue(ChassisPosition tempPosition)
{
  for (int i = 0; i < 4; i++)
  {
    if (positionMarked[i] == tempPosition)
    {
      return positionValues[i];
    }
  }
}

ChassisState Chassis::loop()
{ //true = complete, false = in progress
  //Maximum sensor value ~= 565, use max sensor value if larger
  if (state.hasStopped)
  {
    return state;
  }

  if (targetPosition > positionMax)
  {
    targetPosition = positionMax;
  }

  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter.UpdateFilter(rawSensorValue);
  filteredSensorValue = thisMedianFilter.getFilterValue();

  if (filteredSensorValue != 0)
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
      DriverMotor->run(RELEASE);
      DriverMotor->setSpeed(0);
      state.hasStopped = true;
    }
  }

  return state;
}
