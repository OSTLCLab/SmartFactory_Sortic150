#define ARDUINOJSON_ENABLE_PROGMEM 0 // Arduino Bug https://arduinojson.org/faq/esp32/?utm_source=github&utm_medium=issues
#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>
#include <ChassisDigital.h>
// #include <HandlingUnit.h>
#include <HandlingUnitSerial.h>
#include <MachineLogic.h>
#include <Component.h>
#include <RfidDetector.h>
#include <Receiver.h>
#include <Config.h>
#include <Debug.h>
#include <SPI.h>

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static SoftwareSerial bluetooth{BLUETOOTH_RX, BLUETOOTH_TX};
static NewPing *distanceSensor = new NewPing{CHASSIS_DIGITAL_TRIG_PIN, CHASSIS_DIGITAL_ECHO_PIN, CHASIS_POS_MAX};

static Component<int> *chassis = new ChassisDigital{driverMotor, distanceSensor};
static Component<MachineAPI> *receiver = new Receiver{};
static Component<SortJob> *rfidDetector = new RfidDetector{&partDetector};
static Component<HandlingUnitPosition> *handlingUnit = new HandlingUnitSerial{1000, receiver};
static Component<MachineAPI> *machineLogic = new MachineLogic{handlingUnit, rfidDetector, chassis};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(9600);
  partDetector.PCD_Init();

  receiver->on();
  machineLogic->on();
}

void loop()
{
  receiver->executeOneStep();
  machineLogic->setAction(receiver->getData());
  machineLogic->executeOneStep();
}
