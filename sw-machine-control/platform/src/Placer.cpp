#include <Placer.h>
#include <Debug.h>

PlacerPosition Placer::loop()
{
  //transceive some data
  if (bluetooth.available())
  {
    String response = bluetooth.readStringUntil('\n');
    if (response.startsWith("success("))
    {
      bool parsedValue = (bool)(response.charAt(8) - '0');
      state = parsedValue ? Finish : Invalid;
      componentData = targetValue;

      debugLn("Placerposition[" + String(targetValue) + "]" + "Success[" + String(parsedValue) + "]");
    }
    return componentData;
  }

  // or send some data
  switch (targetValue)
  {
  case PickUpLeft:
    bluetooth.println("pickup(0)");
    break;
  case PickUpRight:
    bluetooth.println("pickup(1)");
    break;
  case DropLeft:
    bluetooth.println("drop(0)");
    break;
  case DropRight:
    bluetooth.println("drop(1)");
    break;
  }

  state = Finish;

  return componentData;
}
