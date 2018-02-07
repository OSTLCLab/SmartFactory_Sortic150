#ifndef SorticMachine_h
#define SorticMachine_h

#include "Chassis.h"
#include "RfidDetector.h"
#include "PlacerPerformance.h"

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
  ChassisState chassisState;
  bool fullStop;
};

class SorticMachine : public Component<SorticMachineState>
{
public:
  SorticMachine(PlacerPerformance *placer,
                RfidDetector *rfidDetector,
                Chassis *chassis,
                Adafruit_MotorShield *currentMotorShield);
  SorticMachineState loop();

  bool arrayByte8Equals(byte a[8], byte b[8]);

private:
  partColor identifyPart();
  PlacerPerformance *placer;
  RfidDetector *rfidDetector;
  Chassis *chassis;
  Adafruit_MotorShield *currentMotorShield;
  SorticMachineState state;
  int step = 0;
  byte teilArraySchwarz[8] = {4, 135, 115, 120, 162, 231, 73, 128}; //{219,69,127,106,217,247,0,0}; //{4, 135, 115, 120, 162, 231, 73, 128};
  byte teilArrayGrau[8] = {4, 42, 117, 211, 162, 231, 73, 128};     //
  byte teilArrayGrauGelb[8] = {4, 161, 115, 94, 162, 231, 73, 128}; //{4, 161, 115, 94, 162, 231, 73, 128};
  byte teilArrayGrauSchwarz[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  partColor currentPartColor;
  ChassisPosition currentDropTarget;
  PlacerActionDirection currentPlaceDirection;
};

#endif
