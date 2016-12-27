#include "SorticVersionSeverin.h"
#include "SorticFramework.h"
#include "MedianFilter.h"

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

SorticMachineSeverin::SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield) : SorticMachine(tempPlacer, tempDetector, tempMover) {
  currentMotorShield = tempMotorShield;
  currentMotorShield -> begin();
}

void SorticMachineSeverin::loop() {
  
}

PlacerSeverin::PlacerSeverin(Adafruit_MotorShield *tempMotorShield) : Placer() {
  currentMotorShield = tempMotorShield;
}



MoverSeverin::MoverSeverin(Adafruit_MotorShield *tempMotorShield) : Mover() {
  currentMotorShield = tempMotorShield;
  rawSensorValue = analogRead(A1);
  thisMedianFilter = MedianFilter(rawSensorValue); //ToDo: Initialize Filter with correct value
  /*
  *BahnMotor = currentMotorShield.getMotor(1);
  
  BahnMotor->setSpeed(0);
  BahnMotor->run(FORWARD);    //FORWARD = Nach Rechts, BACKWARD = Nach Links
  BahnMotor->run(RELEASE);
  */
}

DetectorSeverin::DetectorSeverin() : Detector() {
  
}


