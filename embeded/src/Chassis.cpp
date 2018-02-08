#include "Chassis.h"

#include "Arduino.h"
#include "SorticFramework.h"

Chassis::Chassis(Adafruit_DCMotor *motor, int tempDistanceSensorPin, int startPosition)
{
  DriverMotor = motor;
  distanceSensorPin = tempDistanceSensorPin;
  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter = MedianFilter(rawSensorValue);
  DriverMotor->run(FORWARD); //FORWARD = Nach Rechts, BACKWARD = Nach Links
  DriverMotor->run(RELEASE);
  DriverMotor->setSpeed(0);
  moveToPosition(startPosition);
}

void Chassis::moveToPosition(int newTarget)
{
  state.hasStopped = false;
  targetPosition = newTarget;
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
    if (distanceToTarget < driveTollerance)
    {
      Serial.println("Position[" + String(targetPosition) + "] arrived");
      DriverMotor->run(RELEASE);
      DriverMotor->setSpeed(0);
      state.hasStopped = true;
    }
  }

  return state;
}
