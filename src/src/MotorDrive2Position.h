#include <Adafruit_MotorShield.h>
#include <Command.h>
#include <Sensor.h>

#ifndef motorrun2position_h
#define motorrun2position_h

class MotorDrive2Position : public Command
{
public:
  MotorDrive2Position(Adafruit_DCMotor *motor, Sensor<int> *sensor, int deltaPosition) : sensor{sensor},
                                                                                         motor{motor},
                                                                                         deltaPosition{deltaPosition} {}
  bool doIt(uint8_t *position);

private:
  int calculateDistance(uint8_t *target);
  Sensor<int> *sensor;
  Adafruit_DCMotor *motor;
  int deltaPosition;
};

#endif
