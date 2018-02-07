#ifndef blootoothreciever_h
#define blootoothreciever_h

#include <SoftwareSerial.h>
#include <Sensor.h>

class BlootoothReciever : public Sensor<String>
{
public:
  BlootoothReciever(SoftwareSerial &reciever)
      : reciever{reciever} {}
  bool ready();
  String get();

private:
  SoftwareSerial &reciever;
};

#endif