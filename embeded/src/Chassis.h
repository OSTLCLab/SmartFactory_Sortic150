#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <filters/MedianFilter.h>
#include <Component.h>
#include <ConfigReciever.h>

#ifndef Chassis_h
#define Chassis_h

#define CHASIS_POS_MAX 565
#define DRIVE_TOLERANCE 2
#define MAX_MOTORSPEED 200

struct ChassisState
{
  bool hasStopped = true;
};

class Chassis : public Component<ChassisState>
{
public:
  Chassis(Adafruit_DCMotor *motor, uint8_t distanceSensorPin, int startPosition);

  void moveToPosition(int newTarget);
  ChassisState loop();

private:
  MedianFilter medianFilter;
  uint8_t distanceSensorPin;
  Adafruit_DCMotor *motor;
  ChassisState state;

  int targetPosition;
};

#endif
