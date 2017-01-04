#ifndef MoverSeverin_h
#define MoverSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include "filters/MedianFilter.h"
#include <Adafruit_MotorShield.h>

class MoverSeverin : public Mover
{
  public:
    MoverSeverin(Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin);
    void moveToPosition(MoverPosition newTarget);
    bool moverLoop(); //true = complete, false = in progress
  private:
    MedianFilter thisMedianFilter;
    bool ForwardIsLeft;
    int driveTollerance = 2;
    int maxSpeed = 200;
    int distanceSensorPin;
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
