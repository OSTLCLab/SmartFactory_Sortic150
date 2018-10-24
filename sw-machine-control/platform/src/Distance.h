#pragma once

#include <NewPing.h>
#include <Sensor.h>

class Distance : public Sensor
{
public:
  Distance(NewPing *sensor) : sensor{sensor} {}

  Print &get(Print &out)
  {
    auto value = sensor->ping_cm();
    return value == 0 ? out : out << value;
  }

private:
  NewPing *sensor;
};