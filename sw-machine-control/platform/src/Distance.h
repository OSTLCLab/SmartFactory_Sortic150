#pragma once

#include <NewPing.h>
#include <Sensor.h>
#include <Stream.h>

class Distance : public Sensor
{
public:
  Distance(NewPing *sensor) : sensor{sensor} {}
  Stream &get(Stream &obj)
  {
    obj << String(sensor->ping_cm());
    return obj;
  }

private:
  NewPing *sensor;
};
