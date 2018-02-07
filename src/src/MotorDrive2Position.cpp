#include <Arduino.h>
#include <SendData2Slave.h>
#include <MotorDrive2Position.h>

bool MotorDrive2Position::doIt(uint8_t *position)
{
  //if (!this->sensor->ready())
  {
    // return false;
  }

  int distance = calculateDistance(position);

  if (abs(distance) - this->deltaPosition <= 0)
  {
    //turnOfMotor
    this->motor->setSpeed(0);

    return true;
  }

  uint8_t direction = distance > 0 ? FORWARD : BACKWARD;

  this->motor->run(direction);

  return false;
}

int MotorDrive2Position::calculateDistance(uint8_t *target)
{
  int actualPosition = this->sensor->get();
  int targetPosition{((int)target[0]) + ((int)target[1] << 8) + ((int)target[1] << 16) + ((int)target[1] << 24)};

  Serial.println(actualPosition);

  return actualPosition - targetPosition;
}
