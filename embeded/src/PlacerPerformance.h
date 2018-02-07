#ifndef PlacerPerformance_h
#define PlacerPerformance_h

#include "Arduino.h"
#include "Component.h"
#include <SoftwareSerial.h>
enum class PlacerActionType
{
  none,
  pickUp,
  place
};

enum class PlacerActionDirection
{
  left,
  right,
  front
};

struct PlacerState
{
  int position{};
  float battery{};
  bool hasStopped = true;
  PlacerActionType currentActionType = PlacerActionType::none;
  PlacerActionDirection currentActionDirection = PlacerActionDirection::front;
};

class PlacerPerformance : public Component<PlacerState>
{
public:
  PlacerPerformance(SoftwareSerial *bluetooth);
  void setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection);
  PlacerState loop();

private:
  PlacerState state;
  SoftwareSerial *bluetooth;
  int step;
  unsigned long startTime;
  bool flag_send_complete = false;
  bool flag_receive_complete = false;
  int i_receive_string = 0;
  int last_receive_string = i_receive_string;
  String Send_String[1];
  String Receive_String[5];
  String currentState;
  int currentPosition;
  int direction;
  void bluetoothEvent();
  void bluetoothAction(String action);
};

#endif
