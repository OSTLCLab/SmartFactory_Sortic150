#include "PlacerPerformance.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SoftwareSerial.h>

PlacerPerformance::PlacerPerformance(SoftwareSerial *tempBluetooth) : Placer() {
  bluetooth = tempBluetooth;
  bluetooth->begin(57600);
  this->setAction(PlacerActionType::none, PlacerActionDirection::front);
  Serial.println("Placer Performance is Ready");
  Serial.print("Action Directions: ");
  Serial.print((int)PlacerActionDirection::front);
  Serial.print(" , ");
  Serial.print((int)PlacerActionDirection::left);
  Serial.print(" , ");
  Serial.println((int)PlacerActionDirection::right);
}

void PlacerPerformance::setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection) {
  currentPlacerActionType = newPlacerActionType;
  currentPlacerActionDirection = newPlacerActionDirection;

  Serial.print("New Action: ");
  Serial.print((int)currentPlacerActionType);
  Serial.print(" : ");
  Serial.println((int)currentPlacerActionDirection);
  if(currentPlacerActionType == PlacerActionType::none){
    direction = 0;
    step = 5;
    hasStopped = true;
    bluetooth->write("setMotors(0)\n");
  }
  else {
    hasStopped = false;
    bluetooth->write("setMotors(1)\n");

    if(currentPlacerActionType == PlacerActionType::pickUp) {
      if (currentPlacerActionDirection == PlacerActionDirection::left) {
        Serial.println("Pickup Left");
        direction = 1;
        step = 1;
      }
      if (currentPlacerActionDirection == PlacerActionDirection::right) {
        Serial.println("Pickup Right");
        step = 9;
        direction = -1;
      }
    }
    if(currentPlacerActionType == PlacerActionType::place) {
      if (currentPlacerActionDirection == PlacerActionDirection::left) {
        Serial.println("Drop Left");
        direction = -1;
        step = 4;
      }
      if (currentPlacerActionDirection == PlacerActionDirection::right) {
        Serial.println("Drop Right");
        step = 6;
        direction = 1;
      }
    }
  }
}

bool PlacerPerformance::placerLoop() {
  // Move Placer accoring to action
  if (currentPlacerActionType != PlacerActionType::none) {
    if (currentState == "arrivedPosition" && currentPosition == step) {
      if (step == 5) {
        this->setAction(PlacerActionType::none, PlacerActionDirection::front);
      }
      step += direction;
      // In case of place action move back to pos 5 after placing
      if (step < 1 || step > 9) {
        step = 5;
      }
      delay(500);
    }
    if (currentState != "waiting") {
      bluetoothAction("gotoPosition("+ String(step) +")\n");
    }
    this->bluetoothEvent();
  }
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
