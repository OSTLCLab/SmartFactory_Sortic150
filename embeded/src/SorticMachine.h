#ifndef SorticMachine_h
#define SorticMachine_h

#include "Chassis.h"
#include "RfidDetector.h"
#include "PlacerPerformance.h"
#include "ConfigReciever.h"

#include "Arduino.h"
#include "Component.h"
#include "filters/MedianFilter.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

enum class partColor
{
  none,
  teilSchwarz,
  teilGrau,
  teilGrauGelb,
  teilGrauSchwarz,
  notDeclared
};

enum class MachineJob
{
  idle,
  sorting
};

struct SorticMachineState
{
  MachineJob job;
  PlacerState placerState;
  RfidDetectorState rfidDetectorState;
  Config configState;
  ChassisState chassisState;
};

class SorticMachine : public Component<SorticMachineState>
{
public:
  SorticMachine(PlacerPerformance *placer,
                RfidDetector *rfidDetector,
                Chassis *chassis,
                Adafruit_MotorShield *currentMotorShield,
                ConfigReciever *configReciever);

  SorticMachineState loop();

  bool arrayByte8Equals(byte a[8], byte b[8]);

private:
  PlacerPerformance *placer;
  RfidDetector *rfidDetector;
  Chassis *chassis;
  Adafruit_MotorShield *currentMotorShield;
  ConfigReciever *configReciever;
  SorticMachineState state;
  int targetPosition;
  int getIndexOfRFidChip(Config config, byte detectedRfid[8]);
  int step = 0;
};

#endif
