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
#include <ConfigReciever.h>

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
ConfigReciever *configReciever;

static const RFidChip chips[4] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 400, PlacerActionDirection::left},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 300, PlacerActionDirection::left},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 200, PlacerActionDirection::left},
    {(byte[]){1, 2, 3, 4, 5, 6, 7, 8}, 400, PlacerActionDirection::left}};
void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  //currentPlacer = new PlacerSeverin(PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);

  placer = new PlacerPerformance(&Bluetooth);
  rfidDetector = new RfidDetector(&PartDetector);
  chassis = new Chassis(DriverMotor, DistanceSensorPin, 510);

  Config config{510,
                510,
                (RFidChip *)chips,
                4};

  configReciever = new ConfigReciever{config};

  thisSorticMachine = new SorticMachine(placer, rfidDetector, chassis, &currentMotorShield, configReciever);

  delay(2000);
}

void loop()
{
  thisSorticMachine->loop();
}
