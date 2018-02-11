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
      int motorState = bluetooth.parseInt();

      Serial.println("Motorstate[" + String(motorState) + "]");
      motorIsOn = motorState != 0;
    }
  }
  else
  {
    bluetooth.println("gotoPosition(" + String(data) + ")");
    if (bluetooth.available())
    {
      data = (PlacerPosition)bluetooth.parseInt();

      Serial.println("Placerposition[" + String(actualPosition) + "]");
      if (data == targetPosition)
      {
        bluetooth.println("setMotors(0)");
        motorIsOn = false;
        state = State::Finish;
      }
    }
  }

  return data;
}
