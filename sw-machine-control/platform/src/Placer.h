#ifndef Placer_h
#define Placer_h

#include <Component.h>
#include <SoftwareSerial.h>

enum PlacerPosition
{
  DropLeft = 1,
  PickUpLeft = 4,
  Front = 5,
  PickUpRight = 6,
  DropRight = 9
};

class Placer : public Component<PlacerPosition>
{
public:
  Placer(SoftwareSerial &bluetooth) : bluetooth{bluetooth} {}

protected:
  PlacerPosition loop();

private:
  SoftwareSerial &bluetooth;
};

#endif
