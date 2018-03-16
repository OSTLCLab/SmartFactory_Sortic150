#include <Debug.h>
#include <ArduinoJson.h>
#include <MachineAPI.h>
#include <Placer.h>

Config MachineAPI::loop()
{
  if (!Serial.available())
  {
    state = Finish;
    return componentData;
  }
  StaticJsonBuffer<200> buffer;

  debugLn("Recieve Message.");
  String readedString = Serial.readString();

  JsonObject &root = buffer.parseObject(readedString);

  if (!root.success())
  {
    debugLn("Parsing JSON fail. Bad format.");
    debugLn(readedString);
    buffer.clear();
    state = Invalid;
    return componentData;
  }

  if (root.containsKey(POWER_ON))
  {
    componentData.powerOn = root[POWER_ON];
    debugLn("Power [" + String(componentData.powerOn) + "]");
  }

  if (root.containsKey(CHASSIS_START))
  {
    componentData.chassisStart = root[CHASSIS_START];
    debugLn("chassisStart [" + String(componentData.chassisStart) + "]");
  }

  if (root.containsKey(RFID_SOURCE_POSITION))
  {
    int rfidSourcePosition = root[RFID_SOURCE_POSITION];
    componentData.rfidSourcePosition = (PlacerPosition)rfidSourcePosition;
    debugLn("rfidSourcePosition [" + String(componentData.rfidSourcePosition) + "]");
  }

  if (root.containsKey(PLACER_SLEEP_POSITION))
  {
    int placerSleepPosition = root[PLACER_SLEEP_POSITION];
    componentData.placerSleepPosition = (PlacerPosition)placerSleepPosition;

    debugLn("placerSleepPosition [" + String(componentData.placerSleepPosition) + "]");
  }

  if (root.containsKey(UNKNOWN_POSITION))
  {
    componentData.unknownPosition = root[UNKNOWN_POSITION];
    debugLn("unknownPosition [" + String(componentData.unknownPosition) + "]");
  }

  if (root.containsKey(ID))
  {
    JsonArray &arr = root[ID];
    byte rfid[RFID_LENGTH];
    arr.copyTo(rfid);

    int dest = root[DEST];
    int placer = root[PLACER];
    SortJob newChip{rfid, dest, (PlacerPosition)placer};
    int index = getIndexOfRFidChip(rfid);
    if (index == -1)
    {
      componentData.rfidCount++;
      index = componentData.rfidCount;
    }
    componentData.rfids[index] = newChip;

    debugLn("New RfidChip[" + String(index) + "]");
    debug("RfidChip [" + String(index) + "] id [");
    for (int i = 0; i < RFID_LENGTH; i++)
    {
      debug(newChip.id[i]);

      if (i == RFID_LENGTH - 1)
      {
        debugLn("]");
      }
      else
      {
        debug(",");
      }
    };
  }
  state = Finish;

  return componentData;
}

int MachineAPI::getIndexOfRFidChip(byte *rfId)
{
  for (int index = 0; index < RFID_LENGTH; index++)
  {
    if (!memcmp(rfId, componentData.rfids[index].id, RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }

  return -1;
}