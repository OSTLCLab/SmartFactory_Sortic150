#ifndef ConfigReciever_h
#define ConfigReciever_h

#include <Arduino.h>
#include <PlacerPerformance.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define RFID_LENGTH 8

struct RFidChip
{
  byte *rfidChip;
  int targetPosition;
  PlacerActionDirection targetDirection;

  RFidChip(byte *chips,
           int targetPosition,
           PlacerActionDirection targetDirection) : rfidChip{chips},
                                                    targetPosition{targetPosition},
                                                    targetDirection{targetDirection}
  {
  }
};

struct Config
{
  int startPosition;
  int unknownPosition;
  RFidChip *rfidChips;
  int sizeOfRfidChips;
  bool powerOn;
  Config(
      int startPosition,
      int unknownPosition,
      RFidChip *chips,
      int sizeOfRfidChips,
      bool powerOn) : startPosition{startPosition},
                      unknownPosition{unknownPosition},
                      rfidChips{chips},
                      sizeOfRfidChips{sizeOfRfidChips},
                      powerOn{powerOn}
  {
  }
  Config() : startPosition{},
             unknownPosition{},
             rfidChips{},
             sizeOfRfidChips{},
             powerOn{} {}
};

class ConfigReciever : public Component<Config>
{
public:
  ConfigReciever(Config init) : config{init}, buffer{} {}
  Config loop();

private:
  Config config;
  StaticJsonBuffer<200> buffer;
};

#endif
