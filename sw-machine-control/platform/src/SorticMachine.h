#include <Chassis.h>
#include <RfidDetector.h>
#include <Placer.h>
#include <MachineAPI.h>

#include <Arduino.h>
#include <Component.h>
#include <snippets/MedianFilter.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_MotorShield.h>

#ifndef SorticMachine_h
#define SorticMachine_h

class SorticMachine : public Component<Config>
{
public:
  SorticMachine(Component<PlacerPosition> *placer,
                Component<byte *> *rfidDetector,
                Component<int> *chassis,
                Component<Config> *machineAPI) : placer{placer},
                                                 chassis{chassis},
                                                 rfidDetector{rfidDetector},
                                                 machineAPI{machineAPI}
  {
    this->componentData = machineAPI->getData();
  }

protected:
  Config loop();

private:
  Component<PlacerPosition> *placer;
  Component<int> *chassis;
  Component<byte *> *rfidDetector;
  Component<Config> *machineAPI;
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
