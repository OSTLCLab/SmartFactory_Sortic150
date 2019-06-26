#pragma once

#include <Stream.h>

template <class T>
inline Stream &operator<<(Stream &obj, T arg)
{
  obj.print(arg);
  return obj;
}

class Sensor
{
public:
  virtual Stream &get(Stream &obj) = 0;
};
