#include <Placer.h>
#include <Debug.h>

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
      debugLn("Placerposition[" + String(parsedValue) + "]");
    }
    else
    {
      debugLn(response);
    }
  }

  return sensorData;
}
