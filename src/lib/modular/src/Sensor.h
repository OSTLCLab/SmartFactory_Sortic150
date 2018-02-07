#ifndef sensor_h
#define sensor_h

template <typename T>
class Sensor
{
public:
  virtual bool ready() = 0;
  virtual T get() = 0;
};

#endif
