#ifndef MoverSeverin_h
#define MoverSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include "filters/MedianFilter.h"
#include <Adafruit_MotorShield.h>

class MoverSeverin : public Mover
{
  public:
    MoverSeverin(Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC);
    void moveToPosition(MoverPosition newTarget);
    bool moverLoop(); //true = complete, false = in progress
    int getPositionValue(MoverPosition tempPosition);
  private:
    MedianFilter thisMedianFilter;
    bool ForwardIsLeft = true;
    int driveTollerance = 2;
    int maxSpeed = 200;
    int distanceSensorPin;
    Adafruit_DCMotor *DriverMotor;

    //Position Variables
    MoverPosition positionMarked[4] = {MoverPosition::pickUp, MoverPosition::dropA, MoverPosition::dropB, MoverPosition::dropC};
    int positionValues[4];
    int positionMax = 565;   //Maximum sensor value ~= 565, uses max sensor value if larger
    bool bahnHasStopped;
    int rawSensorValue;
    int filteredSensorValue;
    int targetPosition;
    int currentPosition;
    int distanceToTarget;
};

#endif
