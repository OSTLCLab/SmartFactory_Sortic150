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
  bool getState;
  HandlingUnitPosition handlingUnitStartPosition;
  HandlingUnitPosition sortJobSourcePosition;
};

static const SortJob DEFAULT_SORTJOB{(byte *)((byte[]){0, 0, 0, 0, 0, 0, 0}), -1, NoPosition};

static const Config initialConfig{CHASSIS_POS_START,
                                  {(byte *)((byte[]){0, 0, 0, 0, 0, 0, 0}), -1, NoPosition},
                                  true,
                                  false,
                                  StartPosition,
                                  PickUpRight};

class MachineAPI : public Component<Config>
{
public:
  MachineAPI()
  {
    componentData = initialConfig;
  }

protected:
  Config loop()
  {
    if (!Serial.available())
    {
      return componentData;
    }
    StaticJsonBuffer<250> buffer;
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

    if (root.containsKey(GET_STATE))
    {
      componentData.getState = root[GET_STATE];
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
      componentData.sortJob = SortJob{(byte *)((byte[]){0, 0, 0, 0, 0, 0, 0}), -1, NoPosition};
      componentData.sortJob.handlingUnitPosition = (HandlingUnitPosition)(int)root[HANDLING_UNIT];
      componentData.sortJob.destination = root[DEST];
      JsonArray &arr = root[ID];
      for (int index = 0; index < RFID_LENGTH; index++)
      {
        componentData.sortJob.id[index] = (byte)arr[index];
      }
      root.printTo(Serial);
    }
    return componentData;
  }
};

#endif