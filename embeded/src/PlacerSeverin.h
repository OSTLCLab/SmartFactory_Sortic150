#ifndef PlacerSeverin_h
#define PlacerSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <Adafruit_MotorShield.h>

/*
class PlacerSeverin : public Placer
{
  public:
    PlacerSeverin(Adafruit_DCMotor *tempPlacerMotorBase, Adafruit_DCMotor *tempPlacerMotorArm, Adafruit_DCMotor *tempPlacerMotorClaw);
    void setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection);
    bool placerLoop(); //true = complete, false = in progress

  private:
    Adafruit_DCMotor *PlacerMotorBase;
    Adafruit_DCMotor *PlacerMotorArm;
    Adafruit_DCMotor *PlacerMotorClaw;

    int step; //0 = no step, 1 = turn,  2 = moveDown, 3 = clawAction, 4 = moveUp, 5 = turnBack
    unsigned long OnTime;
    bool hasFinish;

    unsigned long baseQuarterTurnTimePercise = 600;
    unsigned long baseQuarterTurnTimeSave = 2000;
    unsigned long armMoveUpTime = 2100;
    unsigned long armMoveDownTime = 2100;
    unsigned long clawOpenTime = 1200;
    unsigned long clawCloseTime = 1700;
    int driveSpeed = 200;

};
*/
#endif
