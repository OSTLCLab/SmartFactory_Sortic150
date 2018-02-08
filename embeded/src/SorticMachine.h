#include <Chassis.h>
#include <RfidDetector.h>
#include <PlacerPerformance.h>
#include <ConfigReciever.h>

#include <Arduino.h>
#include <Actor.h>
#include <filters/MedianFilter.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#ifndef SorticMachine_h
#define SorticMachine_h

class SorticMachine : public Actor<Config>
{
public:
  SorticMachine(Actor<PlacerPosition> *placer,
                Component<byte *> *rfidDetector,
                Actor<int> *chassis) : placer{placer},
                                       chassis{chassis},
                                       rfidDetector{rfidDetector}
  {
  }

protected:
  Config loop();

private:
  Actor<PlacerPosition> *placer;
  Actor<int> *chassis;
  Component<byte *> *rfidDetector;
  int getIndexOfRFidChip(Config config, byte detectedRfid[8]);
};

#endif
