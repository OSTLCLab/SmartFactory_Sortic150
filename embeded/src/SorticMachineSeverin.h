#ifndef SorticMachineSeverin_h
#define SorticMachineSeverin_h

#include "MoverSeverin.h"
#include "PlacerSeverin.h"
#include "DetectorSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include "filters/MedianFilter.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

enum class partColor {
  none,
  teilSchwarz,
  teilGrau,
  teilGrauGelb,
  teilGrauSchwarz,
  notDeclared
};

class SorticMachineSeverin : public SorticMachine
{
  public:
    SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield);
    void loop();
    partColor identifyPart(byte partArray[]);
    bool arrayByte8Equals(byte a[8], byte b[8]);
  private:
    Adafruit_MotorShield *currentMotorShield;
    int step = 0;
    byte teilArraySchwarz[8] = {4, 135, 115, 120, 162, 231, 73, 128}; //{219,69,127,106,217,247,0,0}; //{4, 135, 115, 120, 162, 231, 73, 128};
    byte teilArrayGrau[8] = {4, 42, 117, 211, 162, 231, 73, 128};//
    byte teilArrayGrauGelb[8] = {4, 161, 115, 94, 162, 231, 73, 128};//{4, 161, 115, 94, 162, 231, 73, 128};
    byte teilArrayGrauSchwarz[8] = {1,2,3,4,5,6,7,8};
    byte currentPart[8];
    partColor currentPartColor;
};

#endif
