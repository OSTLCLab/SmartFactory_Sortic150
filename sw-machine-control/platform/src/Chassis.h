
#include <Adafruit_MotorShield.h>
#include <snippets/MedianFilter.h>
#include <Component.h>

#ifndef Chassis_h
#define Chassis_h

class Chassis : public Component<int>
{
public:
  Chassis(Adafruit_DCMotor *motor, uint8_t DISTANCE_SENSOR) : DISTANCE_SENSOR{DISTANCE_SENSOR},
                                                              motor{motor}
  {
    componentData = 0;
  }

private:
  MedianFilter medianFilter{};
  uint8_t DISTANCE_SENSOR;
  Adafruit_DCMotor *motor;

protected:
  int loop();
};

#endif
