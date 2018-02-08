#ifndef Chassis_h
#define Chassis_h

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <filters/MedianFilter.h>
#include <Component.h>
#include <ConfigReciever.h>

struct ChassisState
{
  bool hasStopped = true;
};

class Chassis : public Component<ChassisState>
{
public:
  Chassis(Adafruit_DCMotor *motor, int tempDistanceSensorPin, int startPosition);

  void moveToPosition(int newTarget);
  ChassisState loop();

private:
  MedianFilter thisMedianFilter;
  bool ForwardIsLeft = true;
  int driveTollerance = 2;
  int maxSpeed = 200;
  int distanceSensorPin;
  Adafruit_DCMotor *DriverMotor;
  ChassisState state;

  //Position Variables
  int positionValues[4];
  int positionMax = 565; //Maximum sensor value ~= 565, uses max sensor value if larger
  int rawSensorValue;
  int filteredSensorValue;
  int targetPosition;
  int currentPosition;
  int distanceToTarget;
};

#endif
