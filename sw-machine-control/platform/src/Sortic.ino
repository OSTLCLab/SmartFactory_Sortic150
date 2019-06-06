#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>
#include <MachineApi.h>
#include <Action.h>
#include <Sensor.h>

#include "Distance.h"
#include "Motor.h"
#include "RfidDetector.h"

static Adafruit_MotorShield motorShield{};
static Adafruit_DCMotor *motor = motorShield.getMotor(MOTOR_NR);
static MFRC522 rfidDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static NewPing *distanceSensor = new NewPing{TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE};

static Sensor *sensors[] = {
    new RfidDetector{&rfidDetector},
    new Distance{distanceSensor}};

static Action *actions[] = {
    new Motor{motor, FORWARD},
    new Motor{motor, BACKWARD},
    new Motor{motor, RELEASE}};

static MachineApi *machineApi = new MachineApi{sensors, NofSensors, actions};

void setup()
{
    Serial.begin(9600);
    motorShield.begin();
    SPI.begin();
    rfidDetector.PCD_Init();
}

void loop()
{
    // s[2(1b0&1c38)1(100)]
    // a0[100]
    machineApi->in(Serial);

    // [s0,s1,...,sn]
    machineApi->out(Serial);
}
