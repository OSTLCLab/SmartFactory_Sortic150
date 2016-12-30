#include "SorticVersionSeverin.h"
#include "SorticFramework.h"
#include "MedianFilter.h"

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

SorticMachineSeverin::SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield) : SorticMachine(tempPlacer, tempDetector, tempMover) {
  currentMotorShield = tempMotorShield;
  currentMotorShield -> begin();
  SPI.begin();
}

void SorticMachineSeverin::loop() {

}


PlacerSeverin::PlacerSeverin(Adafruit_DCMotor *tempPlacerMotorBase, Adafruit_DCMotor *tempPlacerMotorArm, Adafruit_DCMotor *tempPlacerMotorClaw) : Placer() {
  PlacerMotorBase = tempPlacerMotorBase;
  PlacerMotorArm = tempPlacerMotorArm;
  PlacerMotorClaw = tempPlacerMotorClaw;

  PlacerMotorBase->setSpeed(0);
  PlacerMotorBase->run(FORWARD);
  PlacerMotorBase->run(RELEASE);

  PlacerMotorArm->setSpeed(0);
  PlacerMotorArm->run(FORWARD);
  PlacerMotorArm->run(RELEASE);

  PlacerMotorClaw->setSpeed(0);
  PlacerMotorClaw->run(FORWARD);
  PlacerMotorClaw->run(RELEASE);

  hasStopped = true;
  step = 0;
  action = 0;
}

void PlacerSeverin::pickUpLeft() {
  action = 1;
  startTime = millis();
  hasStopped = false;
  step = 1;
}

void PlacerSeverin::pickUpRight() {
  action = 2;
  startTime = millis();
  hasStopped = false;
  step = 1;
}

void PlacerSeverin::placeLeft() {
  action = 3;
  startTime = millis();
  hasStopped = false;
  step = 1;
}

void PlacerSeverin::placeRight() {
  action = 4;
  startTime = millis();
  hasStopped = false;
  step = 1;
}

bool PlacerSeverin::placerLoop() {
  switch(step) { //1 = turn,  2 = moveDown, 3 = clawAction, 4 = moveUp, 5 = turnBack
    case 1:
      if(millis()-startTime>baseQuarterTurnTimeSave) {
        step++;
        PlacerMotorBase->setSpeed(0);
        startTime = millis();
      }
      else if(action == 1 || action == 3) {
        PlacerMotorBase->run(FORWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
      else if(action == 2 || action == 4) {
        PlacerMotorBase->run(BACKWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
    break;

    case 2: //moveDown
      if(millis()-startTime>armMoveDownTime) {
        step++;
        PlacerMotorArm->setSpeed(0);
        startTime = millis();
      }
      else if(action != 0) {
        PlacerMotorArm->run(FORWARD);
        PlacerMotorArm->setSpeed(driveSpeed);
      }
    break;

    case 3: //claw Action
      if(millis()-startTime>clawOpenTime) {
        step++;
        PlacerMotorClaw->setSpeed(0);
        startTime = millis();
      }
      else if(action == 1 || action == 2) {
        PlacerMotorClaw->run(FORWARD);
        PlacerMotorClaw->setSpeed(driveSpeed);
      }
      else if(action == 3 || action == 4) {
        PlacerMotorClaw->run(BACKWARD);
        PlacerMotorClaw->setSpeed(driveSpeed);
      }
    break;

    case 4: //moveUp
      if(millis()-startTime>armMoveUpTime) {
        step++;
        PlacerMotorArm->setSpeed(0);
        startTime = millis();
      }
      else if(action != 0) {
        PlacerMotorArm->run(BACKWARD);
        PlacerMotorArm->setSpeed(driveSpeed);
      }
    break;

    case 5: //turnBack
      if(millis()-startTime>baseQuarterTurnTimeSave) {
        step++;
        PlacerMotorBase->setSpeed(0);
        startTime = millis();
      }
      else if(action == 1 || action == 3) {
        PlacerMotorBase->run(BACKWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
      else if(action == 2 || action == 4) {
        PlacerMotorBase->run(FORWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
    break;

    case 6:
      hasStopped = true;
      action = 0;
      step = 0;
    break;
  }

  return hasStopped;
}

DetectorSeverin::DetectorSeverin(MFRC522 *tempPartDetector) : Detector() {
  PartDetector = tempPartDetector;
  PartDetector.PCD_Init();
}

bool  DetectorSeverin::RfidCardIsPresent() {
  if (PartDetector.PICC_IsNewCardPresent())
  {
    return true;
  }
  return false;
}

void  DetectorSeverin::getPartArray(byte partArray[]) {
  //Example: partArray[0] = 1;
  byte blockAddr = 0;
  byte bufferSize = 8;
  byte outputSize = 8;
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode) PartDetector.MIFARE_Read(blockAddr, partArray, &bufferSize);
  /* Error handling:
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(PartDetector.GetStatusCodeName(status));
  }
  */
}

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
