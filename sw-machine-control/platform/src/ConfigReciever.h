#ifndef ConfigReciever_h
#define ConfigReciever_h

#include <Component.h>
#include <ArduinoJson.h>
#include <Placer.h>

#define RFID_LENGTH 8

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

class ConfigReciever : public Component<Config>
{
public:
  ConfigReciever(Config init)
  {
    this->sensorData = init;
  }

protected:
  Config loop();

private:
  int getIndexOfRFidChip(byte *id);
};

#endif
