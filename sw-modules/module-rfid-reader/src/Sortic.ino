#include <Arduino.h>
#include <MFRC522.h>

#include "Sensor.h"
#include "Config.h"
#include "RfidDetector.h"

static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};

static Sensor *sensors[NofSensors]{
    new RfidDetector{&partDetector}
};

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    partDetector.PCD_Init();
}

void loop()
{
    for (auto index = 0; index < NofSensors; index++)
    {
        sensors[index]->get(Serial) << (index == NofSensors - 1 ? ']' : ',');
    }
}
