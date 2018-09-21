#ifndef HandlingUnitSerial_h
#define HandlingUnitSerial_h

#include <Arduino.h>
#include <Component.h>
#include <Debug.h>
#include <Receiver.h>

class HandlingUnitSerial : public Component<HandlingUnitPosition>
{
public:
  HandlingUnitSerial(unsigned long waitingTime, 
                     Component<MachineAPI> *receiver) :
                                                  waitingTime{waitingTime},
                                                  receiver{receiver},
                                                  millisOfLastSending{0}
  { 
    componentData = StartPosition;
  }

protected:
  HandlingUnitPosition loop()
  {
    if(receiver->getData().actualPosition == targetValue)
    {
      componentData = targetValue;
      state = Finish;
      return targetValue;
    }

    unsigned long actualMillis = millis();
    if (millisOfLastSending + waitingTime > actualMillis)
    {
      return componentData;
    }
    debugLn("Send data!");

    millisOfLastSending = millis();

    // or send some data
    switch (targetValue)
    {
    case PickUpLeft:
      Serial.println("{\"method\":\"pickup\", \"params\":0}");
      break;
    case PickUpRight:
      Serial.println("{\"method\":\"pickup\", \"params\":1}");
      break;
    case DropLeft:
      Serial.println("{\"method\":\"drop\", \"params\":0}");
      break;
    case DropRight:
      Serial.println("{\"method\":\"drop\", \"params\":1}");
      break;
    case StartPosition:
      componentData = targetValue;
      state = Finish;
      break;
    default:
      //do nothing.
      break;
    }

    return componentData;
  }

private:
  unsigned long waitingTime;
  Component<MachineAPI> *receiver;
  unsigned long millisOfLastSending;
};

#endif