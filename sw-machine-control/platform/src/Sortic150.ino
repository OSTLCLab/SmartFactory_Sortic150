#include <Arduino.h>
#include <SPI.h>
#include <SharpIR.h>

#include <MachineLogic.h>
#include <Chassis.h>
#include <Placer.h>
#include <MachineAPI.h>
#include <Component.h>
#include <Config.h>
#include <Debug.h>

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SELECT, RFIDDETECTOR_POWEROFF};
static SoftwareSerial bluetooth{BLUETOOTH_TX, BLUETOOTH_RX};
static SharpIR sensor{GP2Y0A02YK0F, DISTANCE_SENSOR};
static Component<PlacerPosition> *placer = new Placer{bluetooth};
static Component<byte *> *rfidDetector = new RfidDetector{&partDetector};
static Component<int> *chassis = new Chassis{driverMotor, &sensor};
static Component<Config> *machineAPI = new MachineAPI{};
static Component<Config> *machineLogic = new MachineLogic{placer, rfidDetector, chassis, machineAPI};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(57600);
  partDetector.PCD_Init();
  machineAPI->on();
}

void loop()
{
  machineAPI->executeOneStep();
  machineAPI->getData().powerOn ? machineLogic->on() : machineLogic->wait();

  machineLogic->setAction(machineAPI->getData());
  machineLogic->executeOneStep();
  delay(1000);
}
