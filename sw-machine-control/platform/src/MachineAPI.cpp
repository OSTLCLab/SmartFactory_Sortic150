#include <Debug.h>
#include <ArduinoJson.h>
#include <MachineAPI.h>
#include <Placer.h>

Config MachineAPI::loop()
{
  if (!Serial.available())
  {
    state = State::Finish;
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
    state = State::Invalid;
    return componentData;
  }

  if (root.containsKey("powerOn"))
  {
    componentData.powerOn = root["powerOn"];
    debugLn("Power [" + String(componentData.powerOn) + "]");
  }

  if (root.containsKey("chassisStart"))
  {
    componentData.chassisStart = root["chassisStart"];
    debugLn("chassisStart [" + String(componentData.chassisStart) + "]");
  }

  if (root.containsKey("rfidSourcePosition"))
  {
    int rfidSourcePosition = root["rfidSourcePosition"];
    componentData.rfidSourcePosition = (PlacerPosition)rfidSourcePosition;
    debugLn("rfidSourcePosition [" + String(componentData.rfidSourcePosition) + "]");
  }

  if (root.containsKey("placerSleepPosition"))
  {
    int placerSleepPosition = root["placerSleepPosition"];
    componentData.placerSleepPosition = (PlacerPosition)placerSleepPosition;

    debugLn("placerSleepPosition [" + String(componentData.placerSleepPosition) + "]");
  }

  if (root.containsKey("unknownPosition"))
  {
    componentData.unknownPosition = root["unknownPosition"];
    debugLn("unknownPosition [" + String(componentData.unknownPosition) + "]");
  }

  if (root.containsKey("id"))
  {
    JsonArray &arr = root["id"];
    byte rfid[RFID_LENGTH];
    arr.copyTo(rfid);

    int dest = root["dest"];
    int placer = root["placer"];
    RFidChip newChip{rfid, dest, (PlacerPosition)placer};
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
  state = State::Finish;

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