#include <Arduino.h>
#include <DistanceSensor.h>

bool DistanceSensor::ready()
{
  return analogRead(this->distanceSensorPin) != 0;
}
int DistanceSensor::get()
{
  return analogRead(this->distanceSensorPin);
}
