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

class SorticMachineSeverin : public SorticMachine
{
  public:
    SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield);
    void loop();
  private:
    Adafruit_MotorShield *currentMotorShield;
};



#endif
