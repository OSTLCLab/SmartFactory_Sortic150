#include <NewPing.h>
#include <Adafruit_MotorShield.h>
#include <Component.h>
#include <Config.h>
#include <Debug.h>

#ifndef ChassisDigital_h
#define ChassisDigital_h

class ChassisDigital : public Component<int>
{
public:
  ChassisDigital(Adafruit_DCMotor *motor, NewPing *sensor) : sensor{sensor},
                                                             motor{motor}
  {
  }

private:
  NewPing *sensor;
  Adafruit_DCMotor *motor;
  int punish{0};

  bool checkInvalidState(int currentPosition)
  {
    return targetValue > CHASIS_POS_MAX || targetValue < CHASIS_POS_MIN;
  }

  bool checkInvalidPos(int currentPosition)
  {
    return currentPosition > CHASIS_POS_MAX || currentPosition < CHASIS_POS_MIN;
  }

  bool chassisArrivedDestination(int distanceToTarget)
  {
    return distanceToTarget <= CHASSIS_TOLERANCE;
  }

protected:
  int loop()
  {
    int currentPosition = sensor->ping_cm();

    if (checkInvalidPos(currentPosition) && (MAX_MOTORSPEED - punish) > MIN_MOTORSPEED)
    {
      punish += MOTOR_PUNISH_SPEED;
      debugLn("ChassisDigital punished! Motor decrease speed -" + String(MOTOR_PUNISH_SPEED));
      return -1;
    }

    if (checkInvalidState(currentPosition))
    {
      motor->run(RELEASE);
      state = Invalid;
      debugLn("Max exceeded cannot go this way!" + String(currentPosition));

      return currentPosition;
    }

    int distanceToTarget = abs(currentPosition - targetValue);

    motor->run(currentPosition > targetValue ? BACKWARD : FORWARD);
    int maxSpeed = (MAX_MOTORSPEED - punish) > MIN_MOTORSPEED ? (MAX_MOTORSPEED - punish) : MIN_MOTORSPEED;
    if (maxSpeed > MAX_MOTORSPEED)
    {
      maxSpeed = MAX_MOTORSPEED;
    }
    motor->setSpeed(distanceToTarget > 5 ? maxSpeed : (distanceToTarget + MIN_MOTORSPEED));

    if (chassisArrivedDestination(distanceToTarget))
    {
      debugLn("Position arrived!" + String(currentPosition));
      debugLn("ChassisDigital will be rewarded! Motor increases speed +1");
      motor->run(RELEASE);
      state = Finish;
      punish++;
    }

    return currentPosition;
  }
};

#endif