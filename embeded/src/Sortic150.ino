#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

#include <SorticFramework.h>
#include <SorticMachine.h>
#include <Chassis.h>
#include <PlacerSeverin.h>
#include <PlacerPerformance.h>
#include <RfidDetector.h>

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
PlacerPerformance *placer;
RfidDetector *rfidDetector;
Chassis *chassis;
SorticMachine *thisSorticMachine;

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  //currentPlacer = new PlacerSeverin(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);

  placer = new PlacerPerformance(&Bluetooth);
  rfidDetector = new RfidDetector(&PartDetector);
  chassis = new Chassis(DriverMotor, DistanceSensorPin, 510, 400, 300, 200);

  thisSorticMachine = new SorticMachine(placer, rfidDetector, chassis, &currentMotorShield);
  delay(2000);
}

void loop()
{
  //currentMover->moverLoop();
  thisSorticMachine->loop();
}
