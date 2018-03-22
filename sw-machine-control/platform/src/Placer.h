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
  PlacerPosition loop()
  {
    //transceive some data
    if (bluetooth.available())
    {
      String response = bluetooth.readStringUntil('\n');
      if (response.startsWith("success("))
      {
        bool parsedValue = (bool)(response.charAt(8) - '0');
        state = parsedValue ? Finish : Invalid;
        componentData = targetValue;
      }
      return componentData;
    }

    // or send some data
    switch (targetValue)
    {
    case PickUpLeft:
      bluetooth.println("pickup(0)");
      break;
    case PickUpRight:
      bluetooth.println("pickup(1)");
      break;
    case DropLeft:
      bluetooth.println("drop(0)");
      break;
    case DropRight:
      bluetooth.println("drop(1)");
      break;
    }

    return componentData;
  }

private:
  SoftwareSerial &bluetooth;
};

#endif