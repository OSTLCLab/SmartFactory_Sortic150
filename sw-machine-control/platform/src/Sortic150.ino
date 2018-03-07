#include <Arduino.h>
#include <SPI.h>

#include <SorticMachine.h>
#include <Chassis.h>
#include <Placer.h>
#include <ConfigReciever.h>
#include <Component.h>
#include <Actor.h>
#include <Config.h>
#include <Debug.h>

Adafruit_MotorShield currentMotorShield{};
Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
MFRC522 partDetector{RFIDDETECTOR_SELECT, RFIDDETECTOR_POWEROFF};
SoftwareSerial bluetooth{BLUETOOTH_TX, BLUETOOTH_RX};

static const RFidChip chips[8] = {
    {(byte[]){4, 135, 115, 120, 162, 231, 73, 128}, 400, PlacerPosition::PickUpLeft},
    {(byte[]){4, 42, 117, 211, 162, 231, 73, 128}, 300, PlacerPosition::PickUpLeft},
    {(byte[]){4, 161, 115, 94, 162, 231, 73, 128}, 200, PlacerPosition::PickUpLeft},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front},
    {(byte[]){0, 0, 0, 0, 0, 0, 0, 0}, 510, PlacerPosition::Front}};

static Config initialConfig{510,
                            510,
                            (RFidChip *)chips,
                            4,
                            true,
                            PlacerPosition::Front,
                            PlacerPosition::PickUpLeft};

static Actor<PlacerPosition> *placer = new Placer{bluetooth};
static Component<byte *> *rfidDetector = new RfidDetector{partDetector};
static Actor<int> *chassis = new Chassis{driverMotor, DISTANCE_SENSOR};
static Component<Config> *configReciever = new ConfigReciever{initialConfig};
Actor<Config> *sorticMachine = new SorticMachine{placer, rfidDetector, chassis, initialConfig};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(57600);
  partDetector.PCD_Init();
  debugLn("Setup Ready");
}

void loop()
{
  //@Roman: Muss das hier sein, könnte das nicht ins Setup (Nur einmal ausführen)
  // Name conifgRevicever passt mir nicht... ist doch eingelintlich unser Machine API
  configReciever->on();
  configReciever->execute();
  sorticMachine->setAction(configReciever->getData());
  sorticMachine->execute();
  configReciever->getData().powerOn ? sorticMachine->on() : sorticMachine->off();
}
