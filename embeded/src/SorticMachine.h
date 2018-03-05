#include <Chassis.h>
#include <RfidDetector.h>
#include <Placer.h>
#include <ConfigReciever.h>

#include <Arduino.h>
#include <Actor.h>
#include <filters/MedianFilter.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#ifndef SorticMachine_h
#define SorticMachine_h

class SorticMachine : public Actor<Config>
{
public:
  SorticMachine(Actor<PlacerPosition> *placer,
                Component<byte *> *rfidDetector,
                Actor<int> *chassis, Config initialConfig) : placer{placer},
                                                             chassis{chassis},
                                                             rfidDetector{rfidDetector}
  {
    this->sensorData = initialConfig;
  }

protected:
  Config loop();

private:
  Actor<PlacerPosition> *placer;
  Actor<int> *chassis;
  Component<byte *> *rfidDetector;
  int getIndexOfRFidChip();
  void printStatus();
  void printComponentStatus(String name, State state);
  bool chassIsAtStartPosition();
  bool placerIsAtStartPosition();
  bool allOff();
  bool chipDetected();
  bool placerHasChip();
  bool chassisReachedDestination();
  bool allFinished();
};

#endif
