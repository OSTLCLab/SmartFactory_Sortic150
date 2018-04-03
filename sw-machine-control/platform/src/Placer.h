#ifndef Placer_h
#define Placer_h

#include <Component.h>
#include <Debug.h>
#include <SoftwareSerial.h>

enum PlacerPosition
{
  PickUpLeft = 1,
  PickUpRight = 2,
  DropLeft = 3,
  DropRight = 4,
  StartPosition = 5
};

class Placer : public Component<PlacerPosition>
{
public:
  Placer(SoftwareSerial &bluetooth, unsigned long waitingTime) : bluetooth{bluetooth},
                                                                 waitingTime{waitingTime},
                                                                 millisOfLastSending{0}
  {
  }

protected:
  PlacerPosition loop()
  {
    //transceive some data
    if (bluetooth.available())
    {
      String response = bluetooth.readStringUntil('\n');
      debugLn("Recieve some data! " + response);

      if (response.startsWith("success("))
      {
        bool parsedValue = (bool)(response.charAt(8) - '0');
        state = parsedValue ? Finish : Invalid;
        componentData = targetValue;
      }
      return componentData;
    }
    debugLn("Send some data! " + String(targetValue));
    unsigned long actualMillis = millis();
    if (millisOfLastSending + waitingTime > actualMillis)
    {
      return componentData;
    }

    millisOfLastSending = millis();

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
    case StartPosition:
      bluetooth.println("initPosition(1)");
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