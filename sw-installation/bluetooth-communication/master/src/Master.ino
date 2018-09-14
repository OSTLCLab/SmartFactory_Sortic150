#include <Arduino.h>
#include <SoftwareSerial.h>

#define BLUETOOTH_TX 2
#define BLUETOOTH_RX 3

static SoftwareSerial bluetooth{BLUETOOTH_RX, BLUETOOTH_TX};

void setup()
{
    Serial.begin(9600);
    bluetooth.begin(9600);
    bluetooth.println("master ready!");
}

void loop()
{
    if (bluetooth.available()){
       String message = bluetooth.readStringUntil('\n');
        Serial.println(message);     
    }
    bluetooth.println("message from master");
    delay(3000);
}
