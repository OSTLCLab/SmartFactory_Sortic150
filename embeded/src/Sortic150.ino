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
#include <ArduinoJson.h>

#define distanceSensorPin A1
#define bluetoothTx 2
#define bluetoothRx 3
#define rfidDetectorSelectPin 6
#define rfidDetectorPowerDownPin 5
#define motor 1

Adafruit_MotorShield currentMotorShield{};
Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(motor);
MFRC522 partDetector{rfidDetectorSelectPin, rfidDetectorPowerDownPin};
SoftwareSerial bluetooth{bluetoothTx, bluetoothRx};

PlacerPerformance *placer;
RfidDetector *rfidDetector;
Chassis *chassis;
SorticMachine *sorticMachine;
ConfigReciever *configReciever;

static const RFidChip chips[4] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 400, PlacerActionDirection::left},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 300, PlacerActionDirection::left},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 200, PlacerActionDirection::left},
    {(byte[]){1, 2, 3, 4, 5, 6, 7, 8}, 400, PlacerActionDirection::left}};

static const Config initialConfig{510,
                                  510,
                                  (RFidChip *)chips,
                                  4,
                                  true};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();

  placer = new PlacerPerformance{&bluetooth};
  rfidDetector = new RfidDetector{&partDetector};
  chassis = new Chassis{driverMotor, distanceSensorPin, 510};
  configReciever = new ConfigReciever{initialConfig};
  sorticMachine = new SorticMachine{placer, rfidDetector, chassis, &currentMotorShield, configReciever};

  delay(2000);
}

void loop()
{
  sorticMachine->loop();
}
