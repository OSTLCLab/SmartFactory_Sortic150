
#include <Adafruit_MotorShield.h>
#include <SharpIR.h>

#include <Component.h>
#include <Config.h>
#include <Debug.h>

#ifndef Chassis_h
#define Chassis_h

class Chassis : public Component<int>
{
public:
  Chassis(Adafruit_DCMotor *motor, SharpIR *sensor) : sensor{sensor},
                                                      motor{motor}
  {
    componentData = 0;
  }

private:
  SharpIR *sensor;
  Adafruit_DCMotor *motor;

protected:
  int loop()
  {
    int currentPosition = sensor->getDistance();
    if (currentPosition > CHASIS_POS_MAX || currentPosition < CHASIS_POS_MIN)
    {
      motor->run(RELEASE);
      motor->setSpeed(0);
      state = Invalid;
      debugLn("Max or Min pos exceeded!" + String(currentPosition));

      return currentPosition;
    }
    //Maximum sensor value ~= 565, use max sensor value if larger
    if (this->targetValue > CHASIS_POS_MAX)
    {
      this->setAction(CHASIS_POS_MAX);
    }
    int distanceToTarget = abs(currentPosition - this->targetValue);

    motor->run(currentPosition > this->targetValue ? BACKWARD : FORWARD);
    motor->setSpeed(distanceToTarget > 10 ? (MAX_MOTORSPEED / 2) : (distanceToTarget + 50));

    //Stop at target
    if (distanceToTarget <= CHASSIS_TOLERANCE)
    {
      debugLn("Position arrived!" + String(currentPosition));

      motor->run(RELEASE);
      motor->setSpeed(0);
      state = Finish;
    }

    return currentPosition;
  }
};

#endif