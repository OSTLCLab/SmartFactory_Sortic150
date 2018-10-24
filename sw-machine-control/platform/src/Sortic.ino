#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>
#include <MachineApi.h>
#include <Action.h>
#include <Sensor.h>

#include "Distance.h"
#include "Motor.h"
#include "RfidDetector.h"

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static NewPing *newPing = new NewPing{TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE};

static Sensor *sensors[] = {
    new RfidDetector{&partDetector},
    new Distance{newPing}};

static Action *actions[] = {
    new Motor{driverMotor, FORWARD},
    new Motor{driverMotor, BACKWARD},
    new Motor{driverMotor, RELEASE}};

static MachineApi *machineApi = new MachineApi{sensors, NofSensors, actions};

void setup()
{
    Serial.begin(9600);
    currentMotorShield.begin();
    SPI.begin();
    partDetector.PCD_Init();
}

void loop()
{
    machineApi->in(Serial);
    machineApi->out(Serial);
}
