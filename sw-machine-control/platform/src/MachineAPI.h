#ifndef MachineAPI_h
#define MachineAPI_h

#include <Component.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Placer.h>

#define RFID_LENGTH 8

// Ich würde das nicht RFidChip sondern "SortJob" nennen (RfID ist nru grad im moment die Technologie).
struct RFidChip
{
  byte *id;
  int destination;
  PlacerPosition placerPosition;

  RFidChip(byte *chips,
           int destination,
           PlacerPosition placerPosition) : id{chips},
                                            destination{destination},
                                            placerPosition{placerPosition}
  {
  }
};

struct Config
{
  int chassisStart;
  int unknownPosition;
  RFidChip *rfids;
  int rfidCount;
  bool powerOn;
  PlacerPosition placerSleepPosition;
  PlacerPosition rfidSourcePosition;
  Config(
      int chassisStart,
      int unknownPosition,
      RFidChip *chips,
      int rfidCount,
      bool powerOn,
      PlacerPosition placerSleepPosition,
      PlacerPosition rfidSourcePosition) : chassisStart{chassisStart},
                                           unknownPosition{unknownPosition},
                                           rfids{chips},
                                           rfidCount{rfidCount},
                                           powerOn{powerOn},
                                           placerSleepPosition{placerSleepPosition},
                                           rfidSourcePosition{rfidSourcePosition}
  {
  }
  Config() : chassisStart{},
             unknownPosition{},
             rfids{},
             rfidCount{},
             powerOn{} {}
};

static const RFidChip chips[8] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 400, PlacerPosition::PickUpLeft},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 300, PlacerPosition::PickUpLeft},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 200, PlacerPosition::PickUpLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front}};

static Config initialConfig{510,
                            510,
                            (RFidChip *)chips,
                            4,
                            true,
                            PlacerPosition::Front,
                            PlacerPosition::PickUpLeft};

class MachineAPI : public Component<Config>
{
public:
  MachineAPI()
  {
    this->componentData = initialConfig;
  }

protected:
  Config loop();

private:
  int getIndexOfRFidChip(byte *id);
};

#endif
