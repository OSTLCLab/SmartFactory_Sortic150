#ifndef MachineAPI_h
#define MachineAPI_h

#include <Component.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <GrippController.h>
#include <Config.h>

struct SortJob
{
  byte *id;
  int destination;
  GrippPosition grippPosition;
};

struct Config
{
  int chassisStart;
  int unknownPosition;
  SortJob *rfids;
  int rfidCount;
  bool powerOn;
  GrippPosition grippSleepPosition;
  GrippPosition rfidSourcePosition;
};

static const SortJob chips[8] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 40, DropLeft},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 40, DropLeft},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 20, DropRight},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 40, DropLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 40, DropLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 40, DropLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 40, DropLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 40, DropLeft}};

static Config initialConfig{CHASSIS_POS_START,
                            CHASSIS_POS_START,
                            (SortJob *)chips,
                            4,
                            true,
                            StartPosition,
                            PickUpRight};

class MachineAPI : public Component<Config>
{
public:
  MachineAPI()
  {
    this->componentData = initialConfig;
  }

protected:
  Config loop()
  {
    if (!Serial.available())
    {
      state = Finish;
      return componentData;
    }
    StaticJsonBuffer<200> buffer;
    String readedString = Serial.readString();
    JsonObject &root = buffer.parseObject(readedString);

    if (!root.success())
    {
      buffer.clear();
      state = Invalid;
      return componentData;
    }

    if (root.containsKey(POWER_ON))
    {
      componentData.powerOn = root[POWER_ON];
    }

    if (root.containsKey(CHASSIS_START))
    {
      componentData.chassisStart = root[CHASSIS_START];
    }

    if (root.containsKey(RFID_SOURCE_POSITION))
    {
      int rfidSourcePosition = root[RFID_SOURCE_POSITION];
      componentData.rfidSourcePosition = (GrippPosition)rfidSourcePosition;
    }

    if (root.containsKey(GRIPP_SLEEP_POSITION))
    {
      int grippSleepPosition = root[GRIPP_SLEEP_POSITION];
      componentData.grippSleepPosition = (GrippPosition)grippSleepPosition;
    }

    if (root.containsKey(UNKNOWN_POSITION))
    {
      componentData.unknownPosition = root[UNKNOWN_POSITION];
    }

    if (root.containsKey(ID))
    {
      JsonArray &arr = root[ID];
      byte rfid[RFID_LENGTH];
      arr.copyTo(rfid);

      int dest = root[DEST];
      int placer = root[GRIPP];
      SortJob newChip{rfid, dest, (GrippPosition)placer};
      int index = getIndexOfRFidChip(rfid);
      if (index == -1)
      {
        componentData.rfidCount++;
        index = componentData.rfidCount;
      }
      componentData.rfids[index] = newChip;
    }
    state = Finish;

    return componentData;
  }

private:
  int getIndexOfRFidChip(byte *id)
  {
    for (int index = 0; index < RFID_LENGTH; index++)
    {
      if (!memcmp(id, componentData.rfids[index].id, RFID_LENGTH * sizeof(byte)))
      {
        return index;
      }
    }

    return -1;
  }
};

#endif