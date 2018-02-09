#ifndef ConfigReciever_h
#define ConfigReciever_h

#include <Arduino.h>
#include <Component.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <Placer.h>

#define RFID_LENGTH 8

struct RFidChip
{
  byte *rfidChip;
  int targetPosition;
  PlacerPosition placerPosition;

  RFidChip(byte *chips,
           int targetPosition,
           PlacerPosition placerPosition) : rfidChip{chips},
                                            targetPosition{targetPosition},
                                            placerPosition{placerPosition}
  {
  }
};

struct Config
{
  int startPosition;
  int unknownPosition;
  RFidChip *rfidChips;
  int sizeOfRfidChips;
  PlacerPosition placerStartPosition;
  PlacerPosition chipPosition;
  bool powerOn;
  Config(
      int startPosition,
      int unknownPosition,
      RFidChip *chips,
      int sizeOfRfidChips,
      bool powerOn,
      PlacerPosition placerStartPosition,
      PlacerPosition chipPosition) : startPosition{startPosition},
                                     unknownPosition{unknownPosition},
                                     rfidChips{chips},
                                     sizeOfRfidChips{sizeOfRfidChips},
                                     powerOn{powerOn},
                                     placerStartPosition{placerStartPosition},
                                     chipPosition{chipPosition}
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
  ConfigReciever(Config init) : buffer{}
  {
    this->data = init;
  }

protected:
  Config loop();

private:
  StaticJsonBuffer<200> buffer;
};

#endif
