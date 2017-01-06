#include "PlacerSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <Adafruit_MotorShield.h>

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
  Serial.begin(9600);
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
  Serial.print(step);
  Serial.print(" - ");
  Serial.print(startTime);
  Serial.print(" - ");
  Serial.println(millis());
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

      if(action == 1 || action == 2) {
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
      else if(action == 3 || action == 4) {
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
