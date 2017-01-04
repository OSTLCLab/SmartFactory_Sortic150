#include "SorticMachineSeverin.h"

#include "MoverSeverin.h"
#include "PlacerSeverin.h"
#include "DetectorSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h> //Required for setup
#include <MFRC522.h> //Required?
#include <Wire.h> //Required?
#include <Adafruit_MotorShield.h> //Required for setup

SorticMachineSeverin::SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield) : SorticMachine(tempPlacer, tempDetector, tempMover) {
  currentMotorShield = tempMotorShield;
  currentMotorShield -> begin();
  SPI.begin();
}

void SorticMachineSeverin::loop() {

}
