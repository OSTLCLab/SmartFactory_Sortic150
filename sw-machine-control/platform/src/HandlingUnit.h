#ifndef HandlingUnit_h
#define HandlingUnit_h

#include <Component.h>
#include <Debug.h>
#include <SoftwareSerial.h>

enum HandlingUnitPosition
{
  NoPosition = 0,
  PickUpLeft = 1,
  PickUpRight = 2,
  DropLeft = 3,
  DropRight = 4,
  StartPosition = 5
};

class HandlingUnit : public Component<HandlingUnitPosition>
{
public:
  HandlingUnit(SoftwareSerial &bluetooth, unsigned long waitingTime) : bluetooth{bluetooth},
                                                                       waitingTime{waitingTime},
                                                                       millisOfLastSending{0}
  {
      componentData = NoPosition;
  }

protected:
  HandlingUnitPosition loop()
  {
    //transceive some data
    if (bluetooth.available())
    {
      debugLn("Recieve some data!");
      String response = bluetooth.readStringUntil('\n');
      debugLn(response);

      if (response.startsWith("success("))
      {
        bool parsedValue = (bool)(response.charAt(8) - '0');
        state = parsedValue ? Finish : Invalid;

        componentData = targetValue;
      }
      return componentData;
    }
    unsigned long actualMillis = millis();
    if (millisOfLastSending + waitingTime > actualMillis)
    {
      return componentData;
    }

    millisOfLastSending = millis();

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
    case StartPosition:
      bluetooth.println("initPosition(1)");
      break;
    case NoPosition:
      //do nothing.
      break;
    }

    return componentData;
  }

private:
  SoftwareSerial &bluetooth;
  unsigned long waitingTime;
  unsigned long millisOfLastSending;
};

#endif
