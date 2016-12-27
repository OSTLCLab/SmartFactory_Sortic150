#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "SorticFramework.h"
#include "SorticVersionSeverin.h"

Adafruit_MotorShield currentMotorShield = Adafruit_MotorShield();

PlacerSeverin currentPlacer(&currentMotorShield);
DetectorSeverin currentDetector;
MoverSeverin currentMover(&currentMotorShield);

SorticMachineSeverin thisSorticMachine(&currentPlacer, &currentDetector, &currentMover, &currentMotorShield);


void setup() {
  Serial.begin(9600);
}

void loop() {
}
