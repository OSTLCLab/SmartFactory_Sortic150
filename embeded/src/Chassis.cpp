#include <Chassis.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

Chassis::Chassis(Adafruit_DCMotor *motor, uint8_t distanceSensorPin, int startPosition)
{
  this->motor = motor;
  this->distanceSensorPin = distanceSensorPin;
  this->medianFilter = MedianFilter{analogRead(distanceSensorPin)};
  this->motor->run(FORWARD);
  this->motor->run(RELEASE);
  this->motor->setSpeed(0);
  this->moveToPosition(startPosition);
}

void Chassis::moveToPosition(int newTarget)
{
  state.hasStopped = false;
  targetPosition = newTarget;
}

ChassisState Chassis::loop()
{
  //Maximum sensor value ~= 565, use max sensor value if larger
  if (state.hasStopped)
  {
    return state;
  }

  if (targetPosition > CHASIS_POS_MAX)
  {
    targetPosition = CHASIS_POS_MAX;
  }

  int sensorValue = analogRead(distanceSensorPin);
  int currentPosition = medianFilter.getFilterValue(sensorValue);
  int distanceToTarget = abs(currentPosition - targetPosition);

  //set direction
  motor->run(currentPosition > targetPosition ? FORWARD : BACKWARD);

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
    Serial.println("Position[" + String(targetPosition) + "] arrived");
    motor->run(RELEASE);
    motor->setSpeed(0);
    state.hasStopped = true;
  }

  return state;
}
