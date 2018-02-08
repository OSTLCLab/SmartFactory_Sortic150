#ifndef PlacerPerformance_h
#define PlacerPerformance_h

#include <Arduino.h>
#include <Actor.h>
#include <SoftwareSerial.h>
enum class PlacerActionType
{
  none,
  pickUp,
  place
};

enum class PlacerActionDirection
{
  front = 1,
  left = 2,
  right = 4,
};

struct PlacerPosition
{
  PlacerActionType actionType{PlacerActionType::none};
  PlacerActionDirection direction{PlacerActionDirection::front};
};

class PlacerPerformance : public Actor<PlacerPosition>
{
public:
  PlacerPerformance(SoftwareSerial bluetooth) : bluetooth{bluetooth}
  {
  }
  void setPlacer(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection);

protected:
  PlacerPosition loop();

private:
  SoftwareSerial &bluetooth;
  int step;
  bool flag_send_complete = false;
  bool flag_receive_complete = false;
  int i_receive_string = 0;
  int last_receive_string = i_receive_string;
  String Send_String[1];
  String Receive_String[5];
  String currentState;
  int currentPosition;
  PlacerActionType currentActionType = PlacerActionType::none;
  PlacerActionDirection currentActionDirection = PlacerActionDirection::front;
  int direction;
  void bluetoothEvent();
  void bluetoothAction(String action);
};

#endif
