#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>
#include <ChassisDigital.h>
#include <HandlingUnit.h>
#include <MachineLogic.h>
#include <Component.h>
#include <RfidDetector.h>
#include <MachineAPI.h>
#include <Config.h>
#include <Debug.h>
#include <SPI.h>

#include <Ethernet.h>

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static SoftwareSerial bluetooth{BLUETOOTH_TX, BLUETOOTH_RX};
static NewPing *distanceSensor = new NewPing{CHASSIS_DIGITAL_TRIG_PIN, CHASSIS_DIGITAL_ECHO_PIN, CHASIS_POS_MAX};

static Component<HandlingUnitPosition> *handlingUnit = new HandlingUnit{bluetooth, MILLIS_OF_LAST_SENDING};
static Component<int> *chassis = new ChassisDigital{driverMotor, distanceSensor};
static Component<Config> *machineAPI = new MachineAPI{};
static Component<SortJob> *rfidDetector = new RfidDetector{&partDetector};
static Component<Config> *machineLogic = new MachineLogic{handlingUnit, rfidDetector, chassis};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(57600);
  partDetector.PCD_Init();

  machineAPI->on();
  machineLogic->on();
}

void loop()
{
  machineAPI->executeOneStep();
  machineLogic->setAction(machineAPI->getData());
  machineLogic->executeOneStep();
}