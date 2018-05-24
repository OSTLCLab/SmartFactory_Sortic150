#ifndef Receiver_h
#define Receiver_h

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

struct MachineAPI
{
  int chassisStart;
  SortJob sortJob;
  bool powerOn;
  bool getState;
  HandlingUnitPosition startPosition;
  HandlingUnitPosition sortJobSourcePosition;
  HandlingUnitPosition actualPosition;
};

static const SortJob DEFAULT_SORTJOB{(byte *)((byte[]){0, 0, 0, 0, 0, 0, 0}), -1, NoPosition};
static const MachineAPI initialConfig{CHASSIS_POS_START,
                                  {(byte *)((byte[]){0, 0, 0, 0, 0, 0, 0}), -1, NoPosition},
                                  true,
                                  false,
                                  StartPosition,
                                  PickUpRight,
                                  NoPosition};

class Receiver : public Component<MachineAPI>
{
public:
  Receiver()
  {
    componentData = initialConfig;
  }

protected:
  MachineAPI loop()
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
      Serial.println("{\"method\":\"response\", \"params\":{\"success\":false}}");
      return componentData;
    }

    Serial.println("{\"method\":\"response\", \"params\":{\"success\":true}}");

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
      int startPosition = root[HANDLING_UNIT_SLEEP_POSITION];
      componentData.sortJobSourcePosition = (HandlingUnitPosition)startPosition;
    }
    
    if (root.containsKey(HANDLING_UNIT_ACTUAL_POSITION))
    {
      int actualPosition = root[HANDLING_UNIT_ACTUAL_POSITION];
      componentData.actualPosition = (HandlingUnitPosition)actualPosition;
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

    }
    return componentData;
  }
};

#endif
