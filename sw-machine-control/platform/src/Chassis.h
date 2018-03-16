
#include <Adafruit_MotorShield.h>
#include <snippets/MedianFilter.h>
#include <Component.h>
#include <Config.h>

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
  int loop()
  {
    //Maximum sensor value ~= 565, use max sensor value if larger
    if (this->targetValue > CHASIS_POS_MAX)
    {
      this->setAction(CHASIS_POS_MAX);
    }
    int sensorValue = analogRead(DISTANCE_SENSOR);
    medianFilter = MedianFilter{sensorValue};
    int currentPosition = medianFilter.getFilterValue(sensorValue);
    int distanceToTarget = abs(currentPosition - this->targetValue);

    //set direction
    motor->run(currentPosition > this->targetValue ? FORWARD : BACKWARD);

    //Set Speed relative to target
    if (distanceToTarget > 150)
    {
      motor->setSpeed(MAX_MOTORSPEED / 2);
    }
    else
    {
      motor->setSpeed(distanceToTarget + 50);
    }

    //Stop at target
    if (distanceToTarget <= CHASSIS_TOLERANCE)
    {
      motor->run(RELEASE);
      motor->setSpeed(0);
      state = Finish;
    }

    return currentPosition;
  }
};

#endif
