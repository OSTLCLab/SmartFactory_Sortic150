
#include <Adafruit_MotorShield.h>
#include <filters/MedianFilter.h>
#include <Actor.h>

#ifndef Chassis_h
#define Chassis_h

#define CHASIS_POS_MAX 565
#define DRIVE_TOLERANCE 2
#define MAX_MOTORSPEED 200

class Chassis : public Actor<int>
{
public:
  Chassis(Adafruit_DCMotor *motor, uint8_t distanceSensorPin) : distanceSensorPin{distanceSensorPin},
                                                                motor{motor} {}

private:
  MedianFilter medianFilter{};
  uint8_t distanceSensorPin;
  Adafruit_DCMotor *motor;

protected:
  int loop();
};

#endif
