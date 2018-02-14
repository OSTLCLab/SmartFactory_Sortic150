#include <Debug.h>
#include <ArduinoJson.h>
#include <ConfigReciever.h>
#include <Placer.h>

Config ConfigReciever::loop()
{
  if (!Serial.available())
  {
    state = State::Finish;
    return sensorData;
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
    return sensorData;
  }

  if (root.containsKey("powerOn"))
  {
    sensorData.powerOn = root["powerOn"];
    debugLn("Power [" + String(sensorData.powerOn) + "]");
  }

  if (root.containsKey("chassisStart"))
  {
    sensorData.chassisStart = root["chassisStart"];
    debugLn("chassisStart [" + String(sensorData.chassisStart) + "]");
  }

  if (root.containsKey("rfidSourcePosition"))
  {
    int rfidSourcePosition = root["rfidSourcePosition"];
    sensorData.rfidSourcePosition = (PlacerPosition)rfidSourcePosition;
    debugLn("rfidSourcePosition [" + String(sensorData.rfidSourcePosition) + "]");
  }

  if (root.containsKey("placerSleepPosition"))
  {
    int placerSleepPosition = root["placerSleepPosition"];
    sensorData.placerSleepPosition = (PlacerPosition)placerSleepPosition;

    debugLn("placerSleepPosition [" + String(sensorData.placerSleepPosition) + "]");
  }

  if (root.containsKey("unknownPosition"))
  {
    sensorData.unknownPosition = root["unknownPosition"];
    debugLn("unknownPosition [" + String(sensorData.unknownPosition) + "]");
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
      sensorData.rfidCount++;
      index = sensorData.rfidCount;
    }
    sensorData.rfids[index] = newChip;

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

  return sensorData;
}

int ConfigReciever::getIndexOfRFidChip(byte *rfId)
{
  for (int index = 0; index < RFID_LENGTH; index++)
  {
    if (!memcmp(rfId, sensorData.rfids[index].id, RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }

  return -1;
}