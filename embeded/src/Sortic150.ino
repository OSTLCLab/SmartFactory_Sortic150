#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "SorticFramework.h"

#include "SorticMachineSeverin.h"
#include "MoverSeverin.h"
#include "PlacerSeverin.h"
#include "DetectorSeverin.h"

//Sensors:
int DistanceSensorPin = A1;

//Motors:
Adafruit_MotorShield currentMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor *DriverMotor = currentMotorShield.getMotor(1);
Adafruit_DCMotor *PlacerMotorBase = currentMotorShield.getMotor(2);
Adafruit_DCMotor *PlacerMotorArm = currentMotorShield.getMotor(3);
Adafruit_DCMotor *PlacerMotorClaw = currentMotorShield.getMotor(4);

//RFID Detectors:
MFRC522 PartDetector(6,5);

//Components:
PlacerSeverin *currentPlacer;
DetectorSeverin *currentDetector;
MoverSeverin *currentMover;
SorticMachineSeverin *thisSorticMachine;


void setup() {
  //Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();

  currentPlacer = new PlacerSeverin(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);
  currentDetector = new DetectorSeverin(&PartDetector);
  currentMover = new MoverSeverin(DriverMotor, DistanceSensorPin);

  thisSorticMachine = new SorticMachineSeverin(currentPlacer, currentDetector, currentMover, &currentMotorShield);
  delay(5000);
}

void loop() {
  thisSorticMachine->loop();
}
