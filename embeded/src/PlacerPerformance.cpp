#include <PlacerPerformance.h>

#include <Arduino.h>
#include <SorticFramework.h>
#include <SoftwareSerial.h>

void PlacerPerformance::setPlacer(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection)
{
  currentActionType = newPlacerActionType;
  currentActionDirection = newPlacerActionDirection;

  if (currentActionType == PlacerActionType::none)
  {
    state = State::Finish;
    direction = 0;
    step = 5;
    bluetooth.write("setMotors(0)\n");
  }
  else
  {
    state = State::On;
    bluetooth.write("setMotors(1)\n");

    if (currentActionType == PlacerActionType::pickUp)
    {
      if (currentActionDirection == PlacerActionDirection::left)
      {
        Serial.println("Pickup Left");
        direction = 1;
        step = 1;
      }
      if (currentActionDirection == PlacerActionDirection::right)
      {
        Serial.println("Pickup Right");
        step = 9;
        direction = -1;
      }
    }
    if (currentActionType == PlacerActionType::place)
    {
      if (currentActionDirection == PlacerActionDirection::left)
      {
        Serial.println("Drop Left");
        direction = -1;
        step = 4;
      }
      if (currentActionDirection == PlacerActionDirection::right)
      {
        Serial.println("Drop Right");
        step = 6;
        direction = 1;
      }
    }
  }
}

PlacerPosition PlacerPerformance::loop()
{
  // Move Placer accoring to action
  if (currentActionType != PlacerActionType::none)
  {
    if (currentState == "arrivedPosition" && currentPosition == step)
    {
      if (step == 5)
      {
        setPlacer(PlacerActionType::none, PlacerActionDirection::front);
      }
      step += direction;
      // In case of place action move back to pos 5 after placing
      if (step < 1 || step > 9)
      {
        step = 5;
      }
      delay(500);
    }
    if (currentState != "waiting")
    {
      bluetoothAction("gotoPosition(" + String(step) + ")\n");
    }
    this->bluetoothEvent();
  }
  return data;
}

void PlacerPerformance::bluetoothAction(String action)
{
  bluetooth.print(action);
  Serial.println(action);
  currentState = "waiting";
}

void PlacerPerformance::bluetoothEvent()
{
  if (bluetooth.available() > 0 && flag_receive_complete != true)
  {
    delay(1);
    //bluetooth->readStringUntil('\n');

    char inChar = (char)bluetooth.read();
    if (inChar == '(' || inChar == ')' || inChar == '\n' || inChar == ',')
    {
      last_receive_string = i_receive_string;
      i_receive_string = 4;
    }
    Receive_String[i_receive_string] += inChar;
    if (i_receive_string == 4)
    {
      i_receive_string = last_receive_string + 1;
    }
    if (inChar == '\n')
    {
      flag_receive_complete = true;
      currentState = Receive_String[0];
      currentPosition = Receive_String[1].toInt();
      //DEBUG Print bluetooth message
      //Serial.println(currentState + ": " + currentPosition);
      Receive_String[0] = "";
      Receive_String[1] = "";
      Receive_String[2] = "";
      Receive_String[3] = "";
      Receive_String[4] = "";
      i_receive_string = 0;
      flag_receive_complete = false;
    }
  }
}
