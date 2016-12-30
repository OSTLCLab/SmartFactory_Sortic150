#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "SorticFramework.h"
#include "SorticVersionSeverin.h"

Adafruit_MotorShield currentMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor *DriverMotor = currentMotorShield.getMotor(1);
Adafruit_DCMotor *PlacerMotorBase = currentMotorShield.getMotor(2);
Adafruit_DCMotor *PlacerMotorArm = currentMotorShield.getMotor(3);
Adafruit_DCMotor *PlacerMotorClaw = currentMotorShield.getMotor(4);

MFRC522 PartDetector(6,5);

PlacerSeverin currentPlacer(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);
DetectorSeverin currentDetector(&PartDetector);
MoverSeverin currentMover(DriverMotor);

SorticMachineSeverin thisSorticMachine(&currentPlacer, &currentDetector, &currentMover, &currentMotorShield);


void setup() {
  Serial.begin(9600);
}

void loop() {

}
