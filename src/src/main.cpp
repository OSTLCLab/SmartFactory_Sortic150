#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <SendData2Slave.h>
#include <RfidScanner.h>
#include <Sensor.h>
#include <Command.h>
#include <MotorDrive2Position.h>
#include <BlootoothReciever.h>
#include <DistanceSensor.h>
#include <StorageRfid.h>

#define transmitPin 2
#define receivePin 3

#define chipSelectPin 6
#define resetPowerDownPin 5

#define motor 1

#define turnOfMotor "setMotors(0)"
#define turnOnMotor "setMotors(1)"
#define motorGoToPosition "gotoPosition"

#define distanceSensorPin A1

#define startpositionPickUp 510
#define deltaPosition 7

MFRC522 scanner{chipSelectPin, resetPowerDownPin};
static Sensor<uint8_t *> *rfidScanner = new RfidScanner{scanner};

SoftwareSerial bluetoothMaster{transmitPin, receivePin};
static Command *sendData2slave = new SendData2Slave{bluetoothMaster};
static Sensor<String> *bluetoothReciever = new BlootoothReciever{bluetoothMaster};

Adafruit_MotorShield currentMotorShield{};
Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(motor);

static Sensor<int> *distanceSensor = new DistanceSensor{distanceSensorPin};
static Command *run2Position = new MotorDrive2Position{driverMotor, distanceSensor, deltaPosition};

static Command *storageRfid = new StorageRfid{rfidScanner, run2Position, sendData2slave, bluetoothReciever};

enum State
{
    Init = 0,
    Pos0 = 2,
    PosX = 4
};

//State state{Init};

void setup()
{
    //set baudrate
    Serial.begin(9600);
    currentMotorShield.begin();
    SPI.begin();

    scanner.PCD_Init();
    bluetoothMaster.begin(57600);
    driverMotor->setSpeed(0);
}

static bool startPosition = false;
static bool positionX = false;

void loop()
{

    // Start
    if (!(startPosition || positionX))
    {
        uint8_t *value = new uint8_t[4];

        value[0] = (uint8_t)(startpositionPickUp);
        value[1] = (uint8_t)(startpositionPickUp >> 8);
        value[2] = (uint8_t)(startpositionPickUp >> 16);
        value[3] = (uint8_t)(startpositionPickUp >> 24);

        startPosition = run2Position->doIt(value);
        Serial.println("start");
    }

    if (startPosition)
    {
        Serial.println("startPosition");
        uint8_t *rfidAssigment = new uint8_t[4];

        positionX = storageRfid->doIt(rfidAssigment);
    }
}
