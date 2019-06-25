#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>

#include "Action.h"
#include "Sensor.h"
#include "Config.h"
#include "Distance.h"
#include "Motor.h"
#include "RfidDetector.h"

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static NewPing *newPing = new NewPing{CHASSIS_DIGITAL_TRIG_PIN, CHASSIS_DIGITAL_ECHO_PIN, CHASIS_POS_MAX};

static Sensor *sensors[NofSensors]{
    new RfidDetector{&partDetector},
    new Distance{newPing}};
static Action *actions[NofActions]{
    new Motor{driverMotor, FORWARD},
    new Motor{driverMotor, BACKWARD}};

void setup()
{
    Serial.begin(9600);
    currentMotorShield.begin();
    SPI.begin();
    partDetector.PCD_Init();
}

void loop()
{
    if (Serial.available())
    {
        Action *action = actions[Serial.parseInt()];
        action->start(Serial.parseInt());
    }
    Serial << '[';
    for (auto index = 0; index < NofSensors; index++)
    {
        sensors[index]->get(Serial) << (index == NofSensors - 1 ? ']' : ',');
    }
}
