#include "PlacerPerformance.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SoftwareSerial.h>

PlacerPerformance::PlacerPerformance(SoftwareSerial *tempBluetooth) : Placer() {
  bluetooth = tempBluetooth;
  bluetooth->begin(57600);
  delay(1000);
  bluetooth->write("setMotors(0)\n");
  hasStopped = true;
  step = 0;
  Serial.write("Placer Performance is Ready");
  //Serial.begin(9600);
}

void PlacerPerformance::setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection) {
  currentPlacerActionType = newPlacerActionType;
  currentPlacerActionDirection = newPlacerActionDirection;
  if(currentPlacerActionType == PlacerActionType::none){
    bluetooth->write("setMotors(0)\n");
  }
  else {
    //Serial.write("TEST");
    /*bluetooth->write("setMotors(1)\n");
    bluetooth->write("gotoPosition(4)\n");
    delay(5000);
    bluetooth->write("gotoPosition(5)\n");
    delay(5000);*/
    Serial.println("New Job:");
    if (currentPlacerActionType == PlacerActionType::pickUp) {
      bluetooth->write("setMotors(1)\n");;
      startTime = millis();
      hasStopped = false;
      step = 1;
    }
    if (currentPlacerActionType == PlacerActionType::place) {
      hasStopped = true;
      step = 0;
    }
  }
}

bool PlacerPerformance::placerLoop() {
  // Move Placer accoring to action
  //Serial.println("placer: " + step);
  switch(step) { //1 = turn,  2 = moveDown, 3 = clawAction, 4 = moveUp, 5 = turnBack
    case 1: //turn
      //Serial.println("Step 1");
      if (currentPlacerActionType == PlacerActionType::pickUp) {
        if (currentPlacerActionDirection == PlacerActionDirection::left) {
          bluetooth->write("gotoPosition(4)\n");
        } else {
          bluetooth->write("gotoPosition(6)\n");
        }
      }
    break;

    case 2: //moveDown
      //Serial.println("Step 2");
      bluetooth->write("gotoPosition(3)\n");

    break;

    case 3: //claw Action
      bluetooth->write("gotoPosition(2)\n");
    break;

    case 4: //moveUp
      bluetooth->write("gotoPosition(3)\n");
    break;

    case 5: //turnBack
      bluetooth->write("gotoPosition(5)\n");
    break;

    case 6:
      hasStopped = true;
      this->setAction(PlacerActionType::none, PlacerActionDirection::front);
      step = 0;
    break;
  }
  this->bluetoothEvent();
  return hasStopped;
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
    }
  }
  bluetoothControl();
}

void PlacerPerformance::bluetoothControl() {

  if (flag_receive_complete) {
    Serial.println("I have received: ");
    Serial.print("Receive_String[0]: ");
    Serial.println(Receive_String[0]);
    Serial.print("Receive_String[1]: ");
    Serial.println(Receive_String[1]);
    Serial.print("Receive_String[2]: ");
    Serial.println(Receive_String[2]);
    Serial.print("Receive_String[3]: ");
    Serial.println(Receive_String[3]);
    Serial.print("Receive_String[4]: ");
    Serial.println(Receive_String[4]);

    if (Receive_String[0] == "arrivedPosition") {
      step++;
    }

    Receive_String[0] = "";
    Receive_String[1] = "";
    Receive_String[2] = "";
    Receive_String[3] = "";
    Receive_String[4] = "";
    i_receive_string = 0;
    flag_receive_complete = false;
  }
}
