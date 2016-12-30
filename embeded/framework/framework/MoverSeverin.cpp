#include "MoverSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include "MedianFilter.h"

MoverSeverin::MoverSeverin(Adafruit_DCMotor *tempDriverMotor) : Mover() {
  DriverMotor = tempDriverMotor;
  rawSensorValue = analogRead(A1);
  thisMedianFilter = MedianFilter(rawSensorValue); //ToDo: Initialize Filter with correct value

  DriverMotor->setSpeed(0);
  DriverMotor->run(FORWARD);    //FORWARD = Nach Rechts, BACKWARD = Nach Links
  DriverMotor->run(RELEASE);

}

void MoverSeverin::moveToPosition(MoverPosition newTarget) {
  bahnHasStopped = false;
  targetPosition = 150; //ToDo: Create correct target location
}

bool MoverSeverin::moverLoop() { //true = complete, false = in progress
  rawSensorValue = analogRead(A1);
  thisMedianFilter.UpdateFilter(rawSensorValue);
  filteredSensorValue = thisMedianFilter.getFilterValue();

  if((bahnHasStopped == false)and(filteredSensorValue != 0))
  {
    //Funktion fuer Uebertragung in nutzbare grösse
    int currentPosition = filteredSensorValue;
    //Serial.println(currentPosition);

    //In richtige Richtung fahren
    if(currentPosition>targetPosition) {
      //Drive left
      if(ForwardIsLeft) {
        DriverMotor->run(FORWARD);
      }
      else {
        DriverMotor->run(BACKWARD);
      }
    }
    else {
      //Drive right
      if(ForwardIsLeft) {
        DriverMotor->run(BACKWARD);
      }
      else {
        DriverMotor->run(FORWARD);
      }
    }

    distanceToTarget = abs(currentPosition-targetPosition);

    //Set Speed relative to target
    if(distanceToTarget > 150) {
      DriverMotor->setSpeed(maxSpeed);
    }
    else{
      DriverMotor->setSpeed(distanceToTarget+70);
    }

    //Stop at target
    //driveTollerance = distanceToTarget/200+1; //100->2, 200->3, 300->4, 400->5, 500->6
    if(distanceToTarget<driveTollerance) {
      //Serial.println(abs(currentPosition-targetPosition));
      DriverMotor->setSpeed(0);
      bahnHasStopped = true;
    }
  }

  return bahnHasStopped;
}
