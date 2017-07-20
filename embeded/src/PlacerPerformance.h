#ifndef PlacerPerformance_h
#define PlacerPerformance_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <SoftwareSerial.h>

class PlacerPerformance : public Placer
{
  public:
    PlacerPerformance(SoftwareSerial *bluetooth);
    void setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection);
    bool placerLoop(); //true = complete, false = in progress

  private:
    SoftwareSerial *bluetooth;
    int step;
    unsigned long startTime;
    bool hasStopped;
    bool flag_send_complete = false;
    bool flag_receive_complete = false;
    int i_receive_string = 0;
    int last_receive_string = i_receive_string;
    String Send_String[1];
    String Receive_String[5];

    void bluetoothEvent ();
    void bluetoothControl ();
};

#endif
