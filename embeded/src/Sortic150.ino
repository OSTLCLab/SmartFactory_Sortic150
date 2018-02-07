#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

#include <SorticFramework.h>
#include <SorticMachineSeverin.h>
#include <RailMover.h>
#include <PlacerSeverin.h>
#include <PlacerPerformance.h>
#include <BluetoothDetector.h>

//Sensors:
int DistanceSensorPin = A1;

//Motors:
Adafruit_MotorShield currentMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor *DriverMotor = currentMotorShield.getMotor(1);

//RFID Detectors:
MFRC522 PartDetector(6, 5);

//Bluetooth:
SoftwareSerial Bluetooth(2, 3); // TX | RX

//Components:
Placer *currentPlacer;
BluetoothDetector *currentDetector;
RailMover *currentMover;
SorticMachineSeverin *thisSorticMachine;

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  //currentPlacer = new PlacerSeverin(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);

  currentPlacer = new PlacerPerformance(&Bluetooth);
  currentDetector = new BluetoothDetector(&PartDetector);
  currentMover = new RailMover(DriverMotor, DistanceSensorPin, 510, 400, 300, 200);

  thisSorticMachine = new SorticMachineSeverin(currentPlacer, currentDetector, currentMover, &currentMotorShield);
  delay(2000);
}

void loop()
{
  thisSorticMachine->loop();
}
