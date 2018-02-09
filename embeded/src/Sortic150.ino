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
#include <Placer.h>
#include <RfidDetector.h>
#include <ConfigReciever.h>
#include <Component.h>
#include <Actor.h>
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

static const RFidChip chips[4] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 400, PlacerPosition::PickUpLeft},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 300, PlacerPosition::PickUpLeft},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 200, PlacerPosition::PickUpLeft},
    {(byte[]){1, 2, 3, 4, 5, 6, 7, 8}, 400, PlacerPosition::PickUpLeft}};

static const Config initialConfig{510,
                                  510,
                                  (RFidChip *)chips,
                                  4,
                                  true};

static Actor<PlacerPosition> *placer;
static Component<byte *> *rfidDetector;
static Actor<int> *chassis;
static Component<Config> *configReciever;
Actor<Config> *sorticMachine;

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(57600);
  partDetector.PCD_Init();

  placer = new Placer{bluetooth};
  rfidDetector = new RfidDetector{partDetector};
  chassis = new Chassis{driverMotor, distanceSensorPin};
  configReciever = new ConfigReciever{initialConfig};
  sorticMachine = new SorticMachine{placer, rfidDetector, chassis, initialConfig};
  delay(2000);
}

void loop()
{
  configReciever->on();
  sorticMachine->setAction(configReciever->getData());
  sorticMachine->getData();
}
