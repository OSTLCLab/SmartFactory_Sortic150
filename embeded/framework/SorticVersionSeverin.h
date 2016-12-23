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
    PlacerSeverin(Adafruit_MotorShield *tempMotorShield);
    void pickUpLeft();
    void pickUpRight();
    void placeLeft();
    void placeRight();
    bool placerLoop(); //true = complete, false = in progress
  private:
    Adafruit_MotorShield *currentMotorShield;
};

class DetectorSeverin : public Detector
{
  public:
    DetectorSeverin();
    byte * getPartArray(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm  
  private:
    
};

class MoverSeverin : public Mover
{
  public:
    MoverSeverin(Adafruit_MotorShield *tempMotorShield);
    void moveToPosition(MoverPosition newTarget);
    bool moverLoop(); //true = complete, false = in progress
  private:
    MedianFilter thisMedianFilter;
    bool ForwardIsLeft;
    int driveTollerance = 2;
    int maxSpeed = 200;
    Adafruit_DCMotor *BahnMotor;
    
    //Position Variables
    int rawSensorValue;
    int filteredSensorValue;
    int targetPosition;
    int currentPosition;
    int distanceToTarget;
    Adafruit_MotorShield *currentMotorShield;
};



#endif
