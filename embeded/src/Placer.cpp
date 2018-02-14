#include <Placer.h>
#include <Arduino.h>
#include <SorticFramework.h>
#include <SoftwareSerial.h>

PlacerPosition Placer::loop()
{
  if (targetValue == sensorData)
  {
    bluetooth.println("setMotors(0)");
    state = Finish;
    return sensorData;
  }

  bluetooth.println("setMotors(1)");
  bluetooth.println("gotoPosition(" + String(targetValue) + ")");

  if (bluetooth.available())
  {
    String response = bluetooth.readStringUntil('\n');

    if (response.startsWith("arrivedPosition("))
    {
      int parsedValue = (int)(response.charAt(16) - '0');
      sensorData = (PlacerPosition)parsedValue;
      Serial.println("Placerposition[" + String(parsedValue) + "]");
    }
    else
    {
      Serial.println(response);
    }
  }

  return sensorData;
}
