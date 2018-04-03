#include <Arduino.h>
#include <SPI.h>
#include <SharpIR.h>

#include <Chassis.h>
#include <Placer.h>
#include <MachineLogic.h>
#include <Component.h>
#include <RfidDetector.h>
#include <Config.h>
#include <Debug.h>
#include <MFRC522.h>

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static SoftwareSerial bluetooth{BLUETOOTH_TX, BLUETOOTH_RX};
static SharpIR sensor{GP2Y0A02YK0F, DISTANCE_SENSOR};
static Component<PlacerPosition> *placer = new Placer{bluetooth, MILLIS_OF_LAST_SENDING};
static Component<int> *chassis = new Chassis{driverMotor, &sensor};
static Component<Config> *machineAPI = new MachineAPI{};
static Component<int> *rfidDetector = new RfidDetector{&partDetector, machineAPI};
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
}
