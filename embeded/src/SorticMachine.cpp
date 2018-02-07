#include "Chassis.h"
#include "RfidDetector.h"
#include "PlacerPerformance.h"
#include "SorticMachine.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>                  //Required for setup
#include <MFRC522.h>              //Required?
#include <Wire.h>                 //Required?
#include <Adafruit_MotorShield.h> //Required for setup

SorticMachine::SorticMachine(PlacerPerformance *placer,
                             RfidDetector *rfidDetector,
                             Chassis *chassis,
                             Adafruit_MotorShield *currentMotorShield)
{
  this->currentMotorShield = currentMotorShield;
  this->state.job = MachineJob::idle;
  this->placer = placer;
  this->rfidDetector = rfidDetector;
  this->chassis = chassis;
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
  //Serial.println("Maschine Loop");
  //if state == idle
  if (state.job == MachineJob::idle)
  {
    //identify part
    Serial.println("IDLE");
    currentPartColor = identifyPart();

    //if part detected
    if (currentPartColor != partColor::none)
    {
      Serial.println("Detected");

      //set sorting state
      state.job = MachineJob::sorting;
      step = 0;

      //set pickup targets
      switch (currentPartColor)
      {
      case partColor::none:
        Serial.println("none");
        break;

      case partColor::teilSchwarz:
        Serial.println("schwarz");
        currentDropTarget = ChassisPosition::dropA;
        currentPlaceDirection = PlacerActionDirection::left;
        break;

      case partColor::teilGrau:
        Serial.println("grau");
        currentDropTarget = ChassisPosition::dropA;
        currentPlaceDirection = PlacerActionDirection::right;
        break;

      case partColor::teilGrauGelb:
        Serial.println("grauGelb");
        currentDropTarget = ChassisPosition::dropB;
        currentPlaceDirection = PlacerActionDirection::left;
        break;

      case partColor::teilGrauSchwarz:
        Serial.println("grauSchwarz");
        currentDropTarget = ChassisPosition::dropB;
        currentPlaceDirection = PlacerActionDirection::right;
        break;

      case partColor::notDeclared:
        Serial.println("not declared");
        currentDropTarget = ChassisPosition::pickUp;
        currentPlaceDirection = PlacerActionDirection::right;

        //display byte array
        for (int i = 0; i < 8; i++)
        {
          Serial.print(state.rfidDetectorState.partArray[i]);
          Serial.print(" ");
        }
        Serial.println("");
        break;
      }
      //currentPickupTarget = ChasssiPosition::pickUp;
      //currentPickupDirection = PlacerActionDirection::left;
    }
    else
    {
      //Serial.println("No part detected");
    }
  }

  //if state == sorting
  if (state.job == MachineJob::sorting)
  {
    Serial.println(state.chassisState.hasStopped);
    Serial.println(state.placerState.hasStopped);
    if (state.chassisState.hasStopped && state.placerState.hasStopped)
    {
      step++;
      Serial.println(step);

      switch (step)
      {

      case 10:
        chassis->moveToPosition(ChassisPosition::pickUp);
        break;

      case 20:
        //placer->setAction(PlacerActionType::pickUp, PlacerActionDirection::left);
        break;

      case 30:
        chassis->moveToPosition(currentDropTarget);
        break;

      case 40:
        //placer->setAction(PlacerActionType::place, currentPlaceDirection);
        break;

      case 50:
        chassis->moveToPosition(ChassisPosition::pickUp);
        break;

      case 60:
        state.job = MachineJob::idle;
        step = 0;
        break;
      }
    }
  }
}

bool SorticMachine::arrayByte8Equals(byte a[8], byte b[8])
{
  for (int i = 0; i < 7; i++)
  {
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

partColor SorticMachine::identifyPart()
{
  /*
  1) is there a part?
  2) get part array
  3) identify part
  */
  if (rfidDetector->RfidCardIsPresent())
  {
    byte *currentPart = state.rfidDetectorState.partArray;
    if (arrayByte8Equals(currentPart, teilArraySchwarz))
    {
      return partColor::teilSchwarz;
    }
    else if (arrayByte8Equals(currentPart, teilArrayGrau))
    {
      return partColor::teilGrau;
    }
    else if (arrayByte8Equals(currentPart, teilArrayGrauGelb))
    {
      return partColor::teilGrauGelb;
    }
    else if (arrayByte8Equals(currentPart, teilArrayGrauSchwarz))
    {
      return partColor::teilGrauSchwarz;
    }
    else
    {
      return partColor::notDeclared;
    }
  }
  else
  {
    return partColor::none;
  }
}
