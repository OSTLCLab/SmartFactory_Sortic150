#include "Chassis.h"
#include "RfidDetector.h"
#include "PlacerPerformance.h"
#include "SorticMachine.h"
#include "ConfigReciever.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>                  //Required for setup
#include <MFRC522.h>              //Required?
#include <Wire.h>                 //Required?
#include <Adafruit_MotorShield.h> //Required for setup

SorticMachine::SorticMachine(PlacerPerformance *placer,
                             RfidDetector *rfidDetector,
                             Chassis *chassis,
                             Adafruit_MotorShield *currentMotorShield,
                             ConfigReciever *configReciever)
{
  this->currentMotorShield = currentMotorShield;
  this->state.job = MachineJob::idle;
  this->placer = placer;
  this->rfidDetector = rfidDetector;
  this->chassis = chassis;
  this->configReciever = configReciever;
  state.fullStop = false;
}

SorticMachineState SorticMachine::loop()
{
  if (state.fullStop)
  {
    return state;
  }

  delay(10);
  state.placerState = placer->loop();
  state.chassisState = chassis->loop();
  state.rfidDetectorState = rfidDetector->loop();
  state.configState = configReciever->loop();

  if (state.job == MachineJob::idle && state.rfidDetectorState.cardDetected)
  {
    Serial.println("Card detected");

    int rfidIndex = this->getIndexOfRFidChip(state.configState, state.rfidDetectorState.partArray);

    if (rfidIndex != -1)
    {
      Serial.println("Card associated");

      targetPosition = state.configState.rfidChips[rfidIndex].targetPosition;
      state.job = MachineJob::sorting;
      step = 0;
    }
  }

  if (state.job == MachineJob::sorting)
  {
    if (step == 0)
    {
      Serial.println("MachineJob::sorting");
    }
    if (state.chassisState.hasStopped && state.placerState.hasStopped)
    {
      step++;
      switch (step)
      {
      case 10:
        chassis->moveToPosition(state.configState.startPosition);
        break;

      case 30:
        chassis->moveToPosition(targetPosition);
        break;
      case 50:
        chassis->moveToPosition(state.configState.startPosition);
        break;

      case 60:
        state.job = MachineJob::idle;
        step = 0;
        break;
      }
    }
  }
  return state;
}

int SorticMachine::getIndexOfRFidChip(Config config, byte detectedRfid[8])
{
  for (int index = 0; index < config.sizeOfRfidChips; index++)
  {
    RFidChip rfidChip = config.rfidChips[index];

    //foundrfid
    if (!memcmp(rfidChip.rfidChip, detectedRfid, RFID_LENGTH * sizeof(byte)))
    {
      return index;
    }
  }
  return -1;
}