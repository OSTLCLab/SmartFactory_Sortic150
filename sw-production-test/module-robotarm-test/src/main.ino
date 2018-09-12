#include <Arduino.h>
#include <SoftwareSerial.h>

int pick = true;

enum Action
{
  IDLE,
  PICKUP,
  DROP,
  WAITING
};

//Bluetooth:
SoftwareSerial Bluetooth(2, 3); // TX | RX
Action currentState = IDLE;


void setup() {
  Serial.begin(9600);
  Bluetooth.begin(57600);
  delay(2000);
  Serial.println("Ready to start");
}

void loop() {
  if (currentState == IDLE) {
    if (pick) {
      pick = false;
      bluetoothAction("pickup();");
    } else
    {
      pick = true;
      bluetoothAction("drop();");
    }
  }
  bluetoothEvent();
}

void bluetoothAction (String action) {
    Bluetooth.print(action);
    Serial.println(action);
    currentState = WAITING;
}

void bluetoothEvent() {
  if (Bluetooth.available()) {
    String req = Bluetooth.readStringUntil('\n');
    if (req.startsWith("success(1)")) {
      currentState = IDLE;
    }
    Serial.print("Revcieved: ");
    Serial.println(req);
  }
}
