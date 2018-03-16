#ifndef Placer_h
#define Placer_h

#include <Component.h>
#include <SoftwareSerial.h>

enum PlacerPosition
{
  PickUpLeft = 1,
  PickUpRight = 2,
  DropLeft = 3,
  DropRight = 4
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
