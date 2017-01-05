#ifndef PlacerSeverin_h
#define PlacerSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <Adafruit_MotorShield.h>

class PlacerSeverin : public Placer
{
  public:
    PlacerSeverin(Adafruit_DCMotor *tempPlacerMotorBase, Adafruit_DCMotor *tempPlacerMotorArm, Adafruit_DCMotor *tempPlacerMotorClaw);
    void pickUpLeft();
    void pickUpRight();
    void placeLeft();
    void placeRight();
    bool placerLoop(); //true = complete, false = in progress

  private:
    Adafruit_DCMotor *PlacerMotorBase;
    Adafruit_DCMotor *PlacerMotorArm;
    Adafruit_DCMotor *PlacerMotorClaw;

    int step; //0 = no step, 1 = turn,  2 = moveDown, 3 = clawAction, 4 = moveUp, 5 = turnBack
    int action; // 0 = no action, 1 = pickUpLeft, 2 = pickUpRight, 3 = placeLeft, 4 = placeRight
    unsigned long startTime;
    bool hasStopped;

    unsigned long baseQuarterTurnTimePercise = 800;
    unsigned long baseQuarterTurnTimeSave = 1500;
    unsigned long armMoveUpTime = 2100;
    unsigned long armMoveDownTime = 2800;
    unsigned long clawOpenTime = 900;
    unsigned long clawCloseTime = 1500;
    int driveSpeed = 200;

};

#endif
