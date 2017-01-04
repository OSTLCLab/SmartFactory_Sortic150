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
PlacerSeverin currentPlacer(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);
DetectorSeverin currentDetector(&PartDetector);
MoverSeverin currentMover(DriverMotor, DistanceSensorPin);

SorticMachineSeverin thisSorticMachine(&currentPlacer, &currentDetector, &currentMover, &currentMotorShield);


void setup() {
  Serial.begin(9600);
}

void loop() {
  thisSorticMachine.loop();

}
