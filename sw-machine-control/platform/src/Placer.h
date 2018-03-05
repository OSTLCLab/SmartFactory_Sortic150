#ifndef Placer_h
#define Placer_h

#include <Actor.h>
#include <SoftwareSerial.h>

enum PlacerPosition
{
  PickUpLeft = 1,
  DropLeft = 4,
  Front = 5,
  DropRight = 6,
  PickUpRight = 9
};

class Placer : public Actor<PlacerPosition>
{
public:
  Placer(SoftwareSerial &bluetooth) : bluetooth{bluetooth} {}

protected:
  PlacerPosition loop();

private:
  // todo.. weshalb soll der MOTOR_NR hier und nicht beim greifarm selbst reguliert werden?
  SoftwareSerial &bluetooth;
};

#endif
