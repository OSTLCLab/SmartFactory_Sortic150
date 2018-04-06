#ifndef MachineAPI_h
#define MachineAPI_h

#include <Component.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HandlingUnit.h>
#include <Config.h>

struct SortJob
{
  byte *id;
  int destination;
  HandlingUnitPosition handlingUnitPosition;
};

struct Config
{
  int chassisStart;
  SortJob sortJob;
  bool powerOn;
  HandlingUnitPosition handlingUnitStartPosition;
  HandlingUnitPosition sortJobSourcePosition;
};

static SortJob DEFAULT_SORTJOB{(byte[]){0, 0, 0, 0, 0, 0, 0}, -1, NoPosition};

static Config initialConfig{CHASSIS_POS_START,
                            {(byte[]){0, 0, 0, 0, 0, 0, 0}, -1, NoPosition},
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
      return componentData;
    }
    StaticJsonBuffer<200> buffer;
    String readedString = Serial.readString();
    JsonObject &root = buffer.parseObject(readedString);

    if (!root.success())
    {
      buffer.clear();
      state = Invalid;
      Serial.println("success(0)");
      return componentData;
    }

    Serial.println("success(1)");

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
      int sortJobSourcePosition = root[RFID_SOURCE_POSITION];
      componentData.sortJobSourcePosition = (HandlingUnitPosition)sortJobSourcePosition;
    }

    if (root.containsKey(HANDLING_UNIT_SLEEP_POSITION))
    {
      int handlingUnitStartPosition = root[HANDLING_UNIT_SLEEP_POSITION];
      componentData.sortJobSourcePosition = (HandlingUnitPosition)handlingUnitStartPosition;
    }

    if (root.containsKey(ID))
    {
      JsonArray &arr = root[ID];
      byte rfid[RFID_LENGTH];
      arr.copyTo(rfid);

      int dest = root[DEST];
      int placer = root[HANDLING_UNIT];
      SortJob newChip{rfid, dest, (HandlingUnitPosition)placer};
      componentData.sortJob = newChip;
      root.printTo(Serial);
    }
    return componentData;
  }
};

#endif