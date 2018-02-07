#ifndef Chassis_h
#define Chassis_h

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <filters/MedianFilter.h>
#include <Component.h>

struct ChassisState
{
  bool hasStopped = true;
};

enum class ChassisPosition
{
  pickUp,
  dropA,
  dropB,
  dropC
};

class Chassis : public Component<ChassisState>
{
public:
  Chassis(Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC);
  void moveToPosition(ChassisPosition newTarget);
  ChassisState loop(); //true = complete, false = in progress
  int getPositionValue(ChassisPosition tempPosition);

private:
  MedianFilter thisMedianFilter;
  bool ForwardIsLeft = true;
  int driveTollerance = 2;
  int maxSpeed = 200;
  int distanceSensorPin;
  Adafruit_DCMotor *DriverMotor;
  ChassisState state;

  //Position Variables
  ChassisPosition positionMarked[4] = {ChassisPosition::pickUp, ChassisPosition::dropA, ChassisPosition::dropB, ChassisPosition::dropC};
  int positionValues[4];
  int positionMax = 565; //Maximum sensor value ~= 565, uses max sensor value if larger
  int rawSensorValue;
  int filteredSensorValue;
  int targetPosition;
  int currentPosition;
  int distanceToTarget;
};

#endif
