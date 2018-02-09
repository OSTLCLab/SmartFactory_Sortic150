#include <Placer.h>
#include <Arduino.h>
#include <SorticFramework.h>
#include <SoftwareSerial.h>

PlacerPosition Placer::loop()
{
  if (!motorIsOn)
  {
    bluetooth.println("setMotors(1)");
    if (bluetooth.available())
    {
      Serial.println(bluetooth.readString());
      int motorState = bluetooth.parseInt();

      Serial.println("Motorstate[" + String(motorState) + "]");
      motorIsOn = motorState != 0;
    }
  }
  else
  {
    bluetooth.println("gotoPosition(" + String(data) + ")");
    bluetooth.println("getPosition()");

    if (bluetooth.available())
    {
      int actualPosition = bluetooth.parseInt();

      Serial.println("Placerposition[" + String(actualPosition) + "]");
      if (actualPosition == (int)data)
      {
        bluetooth.println("setMotors(0)");
        motorIsOn = false;
        state = State::Finish;
      }
    }
  }

  return data;
}
