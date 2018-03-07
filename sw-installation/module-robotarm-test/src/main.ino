#include <Arduino.h>
#include <SoftwareSerial.h>

//Bluetooth:
SoftwareSerial Bluetooth(2, 3); // TX | RX
int step;
int direction;
bool flag_send_complete = false;
bool flag_receive_complete = false;
int i_receive_string = 0;
int last_receive_string = i_receive_string;
String Send_String[1];
String Receive_String[5];
String currentState;
int currentPosition;

void setup() {
  Serial.begin(9600);
  Bluetooth.begin(57600);
  delay(2000);
  Bluetooth.write("setMotors(1)\n");
  currentPosition = -1; // unknown;
  currentState = "idle";
  step = 4;
  direction = 1;
  bluetoothAction("gotoPosition(4)\n");
}

void loop() {
  if (currentState == "arrivedPosition" && currentPosition == step) {
    delay(2000);
    step += direction;
    if (step == 9) {
      direction = -1;
    }
    if (step == 0) {
      direction = 1;
    }
  }
  if (currentState != "waiting") {
    bluetoothAction("gotoPosition("+ String(step) +")\n");
  }
  bluetoothEvent();
}

void bluetoothAction (String action) {
    Bluetooth.print(action);
    Serial.println(action);
    currentState = "waiting";
}

void bluetoothEvent() {
  if (Bluetooth.available() > 0 && flag_receive_complete != true) {
    delay(1);
    char inChar = (char)Bluetooth.read();
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
      //printBlueMessage();
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

void printBlueMessage() {
  Serial.println("I have received: ");
  Serial.println(Receive_String[0] + "(" + Receive_String[1] + ", " + Receive_String[2] + ", " + Receive_String[3] + ", " + Receive_String[4] + ")");
}
