#ifndef ConfigReciever_h
#define ConfigReciever_h

#include <Arduino.h>
#include <Arduino.h>
#include <PlacerPerformance.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

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
  Config(
      int startPosition,
      int unknownPosition,
      RFidChip *chips,
      int sizeOfRfidChips) : startPosition{startPosition},
                             unknownPosition{unknownPosition},
                             rfidChips{chips},
                             sizeOfRfidChips{sizeOfRfidChips}
  {
  }
  Config() : startPosition{},
             unknownPosition{},
             rfidChips{},
             sizeOfRfidChips{} {}
};

class ConfigReciever : public Component<Config>
{
public:
  ConfigReciever(Config init) : config{init} {}
  Config loop();

private:
  Config config;
};

#endif
