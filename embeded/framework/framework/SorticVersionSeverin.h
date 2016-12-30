#ifndef SorticVersionSeverin_h
#define SorticVersionSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include "MedianFilter.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

class SorticMachineSeverin : public SorticMachine
{
  public:
    SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield);
    void loop();
  private:
    Adafruit_MotorShield *currentMotorShield;
};

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

    unsigned long baseQuarterTurnTimePercise = 1500;
    unsigned long baseQuarterTurnTimeSave = 1500;
    unsigned long armMoveUpTime = 2100;
    unsigned long armMoveDownTime = 2800;
    unsigned long clawOpenTime = 900;
    unsigned long clawCloseTime = 1500;
    int driveSpeed = 200;

};

class DetectorSeverin : public Detector
{
  public:
    DetectorSeverin(MFRC522 *tempPartDetector);
    bool RfidCardIsPresent();
    void getPartArray(byte partArray[]); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
  private:
    MFRC522 PartDetector;



};

class MoverSeverin : public Mover
{
  public:
    MoverSeverin(Adafruit_DCMotor *tempDriverMotor);
    void moveToPosition(MoverPosition newTarget);
    bool moverLoop(); //true = complete, false = in progress
  private:
    MedianFilter thisMedianFilter;
    bool ForwardIsLeft;
    int driveTollerance = 2;
    int maxSpeed = 200;
    Adafruit_DCMotor *DriverMotor;

    //Position Variables
    bool bahnHasStopped;
    int rawSensorValue;
    int filteredSensorValue;
    int targetPosition;
    int currentPosition;
    int distanceToTarget;
};



#endif
