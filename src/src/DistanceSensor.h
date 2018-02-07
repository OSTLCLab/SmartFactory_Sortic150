#ifndef distancesensor_h
#define distancesensor_h

#include <Arduino.h>
#include <Sensor.h>

class DistanceSensor : public Sensor<int>
{
public:
  DistanceSensor(uint8_t distanceSensorPin)
      : distanceSensorPin{distanceSensorPin} {}
  bool ready();
  int get();

private:
  uint8_t distanceSensorPin;
};

#endif