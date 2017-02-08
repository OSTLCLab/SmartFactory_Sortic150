#include "PlacerSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <Adafruit_MotorShield.h>

PlacerSeverin::PlacerSeverin(Adafruit_DCMotor *tempPlacerMotorBase, Adafruit_DCMotor *tempPlacerMotorArm, Adafruit_DCMotor *tempPlacerMotorClaw) : Placer() {
  PlacerMotorBase = tempPlacerMotorBase;
  PlacerMotorArm = tempPlacerMotorArm;
  PlacerMotorClaw = tempPlacerMotorClaw;

  PlacerMotorBase->run(FORWARD);
  PlacerMotorBase->run(RELEASE);
  PlacerMotorBase->setSpeed(0);

  PlacerMotorArm->run(FORWARD);
  PlacerMotorArm->run(RELEASE);
  PlacerMotorArm->setSpeed(0);

  PlacerMotorClaw->run(FORWARD);
  PlacerMotorClaw->run(RELEASE);
  PlacerMotorClaw->setSpeed(0);

  hasStopped = true;
  step = 0;
  //Serial.begin(9600);
}

void PlacerSeverin::setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection) {
  currentPlacerActionType = newPlacerActionType;
  currentPlacerActionDirection = newPlacerActionDirection;
  if(currentPlacerActionType == PlacerActionType::none){
    PlacerMotorClaw->run(FORWARD);
    PlacerMotorArm->run(RELEASE);
    PlacerMotorBase->run(RELEASE);
  }
  else {
    startTime = millis();
    hasStopped = false;
    step = 1;
  }
}

bool PlacerSeverin::placerLoop() {
  switch(step) { //1 = turn,  2 = moveDown, 3 = clawAction, 4 = moveUp, 5 = turnBack
    case 1: //turn
      if((millis()-startTime>baseQuarterTurnTimeSave)||(currentPlacerActionDirection == PlacerActionDirection::front)) {
        step++;
        PlacerMotorBase->setSpeed(0);
        startTime = millis();
      }
      else if(currentPlacerActionDirection == PlacerActionDirection::left) {
        PlacerMotorBase->run(FORWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
      else if(currentPlacerActionDirection == PlacerActionDirection::right) {
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
      else if(currentPlacerActionType != PlacerActionType::none) {
        PlacerMotorArm->run(FORWARD);
        PlacerMotorArm->setSpeed(driveSpeed);
      }
    break;

    case 3: //claw Action

      if(currentPlacerActionType == PlacerActionType::pickUp) {
        if(millis()-startTime>clawCloseTime) {
          step++;
          PlacerMotorClaw->setSpeed(0);
          startTime = millis();
        }
        else {
          PlacerMotorClaw->run(BACKWARD);
          PlacerMotorClaw->setSpeed(driveSpeed);
        }
      }
      else if(currentPlacerActionType == PlacerActionType::place) {
        if(millis()-startTime>clawOpenTime) {
          step++;
          PlacerMotorClaw->setSpeed(0);
          startTime = millis();
        }
        else {
          PlacerMotorClaw->run(FORWARD);
          PlacerMotorClaw->setSpeed(driveSpeed);
        }

      }
    break;

    case 4: //moveUp
      if(millis()-startTime>armMoveUpTime) {
        step++;
        PlacerMotorArm->setSpeed(0);
        startTime = millis();
      }
      else if(currentPlacerActionType != PlacerActionType::none) {
        PlacerMotorArm->run(BACKWARD);
        PlacerMotorArm->setSpeed(driveSpeed);
      }
    break;

    case 5: //turnBack
      if((millis()-startTime>baseQuarterTurnTimeSave)||(currentPlacerActionDirection == PlacerActionDirection::front)) {
        step++;
        PlacerMotorBase->setSpeed(0);
      }
      else if(currentPlacerActionDirection == PlacerActionDirection::left) {
        PlacerMotorBase->run(BACKWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
      else if(currentPlacerActionDirection == PlacerActionDirection::right) {
        PlacerMotorBase->run(FORWARD);
        PlacerMotorBase->setSpeed(driveSpeed);
      }
    break;

    case 6:
      hasStopped = true;
      currentPlacerActionType = PlacerActionType::none;
      step = 0;
    break;
  }

  return hasStopped;
}
