#include "PlacerPerformance.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SoftwareSerial.h>

PlacerPerformance::PlacerPerformance(SoftwareSerial *tempBluetooth) : Placer() {
  bluetooth = tempBluetooth;
  bluetooth->begin(57600);
  delay(2000);
  this->setAction(PlacerActionType::none, PlacerActionDirection::front);
  Serial.write("Placer Performance is Ready");
}

void PlacerPerformance::setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection) {
  currentPlacerActionType = newPlacerActionType;
  currentPlacerActionDirection = newPlacerActionDirection;
  if(currentPlacerActionType == PlacerActionType::none){
    direction = 0;
    step = 5;
    hasStopped = true;
    bluetooth->write("setMotors(0)\n");
  }
  else {
    //bluetooth->write("setMotors(1)\n");
    //Serial.write("TEST");
    /*bluetooth->write("setMotors(1)\n");
    bluetooth->write("gotoPosition(4)\n");
    delay(5000);
    bluetooth->write("gotoPosition(5)\n");
    delay(5000);*/
    hasStopped = false;
    bluetooth->write("setMotors(1)\n");

    if(currentPlacerActionType == PlacerActionType::pickUp) {
      if (currentPlacerActionDirection == PlacerActionDirection::left) {
        Serial.println("New Job: Pickup Left");
        direction = 1;
        step = 1;
      }
      if (currentPlacerActionDirection == PlacerActionDirection::right) {
        Serial.println("New Job: Pickup Right");
        step = 9;
        direction = -1;
      }
    }
  }
}

bool PlacerPerformance::placerLoop() {
  // Move Placer accoring to action
  if (currentState == "arrivedPosition" && currentPosition == step) {
    if (step == 5) {
      this->setAction(PlacerActionType::none, PlacerActionDirection::front);
    }
    step += direction;
    delay(100);
  }
  if (currentState != "waiting") {
    bluetoothAction("gotoPosition("+ String(step) +")\n");
  }

  this->bluetoothEvent();
  return hasStopped;
}

void PlacerPerformance::bluetoothAction (String action) {
    bluetooth->print(action);
    Serial.println(action);
    currentState = "waiting";
}

void PlacerPerformance::bluetoothEvent() {
  if (bluetooth->available() > 0 && flag_receive_complete != true) {
    delay(1);
    char inChar = (char)bluetooth->read();
    if (inChar == '(' || inChar == ')' || inChar == '\n' || inChar == ',') {
      last_receive_string = i_receive_string;
      i_receive_string = 4;
    }
    Receive_String[i_receive_string] += inChar;
    if (i_receive_string == 4) {
      i_receive_string = last_receive_string + 1;
    }
    if (inChar == '\n') {
      flag_receive_complete = true;
      currentState = Receive_String[0];
      currentPosition = Receive_String[1].toInt();
      Serial.println(currentState + ": " + currentPosition);
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
