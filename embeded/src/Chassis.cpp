#include <Chassis.h>
#include <Arduino.h>

int Chassis::loop()
{
  //Maximum sensor value ~= 565, use max sensor value if larger
  if (this->targetValue > CHASIS_POS_MAX)
  {
    this->setAction(CHASIS_POS_MAX);
  }

  int sensorValue = analogRead(distanceSensorPin);
  int currentPosition = medianFilter.getFilterValue(sensorValue);
  int distanceToTarget = abs(currentPosition - this->targetValue);

  //set direction
  motor->run(currentPosition > this->targetValue ? FORWARD : BACKWARD);

  //Set Speed relative to target
  if (distanceToTarget > 150)
  {
    motor->setSpeed(MAX_MOTORSPEED);
  }
  else
  {
    motor->setSpeed(distanceToTarget + 70);
  }

  //Stop at target
  if (distanceToTarget < DRIVE_TOLERANCE)
  {
    Serial.println("Position[" + String(this->targetValue) + "] arrived");
    motor->run(RELEASE);
    motor->setSpeed(0);
    state = State::Finish;
  }

  return sensorValue;
}
