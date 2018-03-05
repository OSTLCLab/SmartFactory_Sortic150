#include <Chassis.h>
#include <Debug.h>
#include <Arduino.h>

int Chassis::loop()
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

  debugLn("Actual Position[" + String(currentPosition) + "] arrived");

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
    debugLn("Position[" + String(this->targetValue) + "] arrived");
    motor->run(RELEASE);
    motor->setSpeed(0);
    state = State::Finish;
  }

  return currentPosition;
}
