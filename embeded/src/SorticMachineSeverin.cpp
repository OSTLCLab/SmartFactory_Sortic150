#include "SorticMachineSeverin.h"

#include "MoverSeverin.h"
#include "PlacerSeverin.h"
#include "DetectorSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h> //Required for setup
#include <MFRC522.h> //Required?
#include <Wire.h> //Required?
#include <Adafruit_MotorShield.h> //Required for setup

SorticMachineSeverin::SorticMachineSeverin(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover, Adafruit_MotorShield *tempMotorShield) : SorticMachine(tempPlacer, tempDetector, tempMover) {
  currentMotorShield = tempMotorShield;
  //currentMotorShield -> begin();
  //SPI.begin();
  currentMachineLogicState = MachineLogicState::idle;
  currentPlacer = tempPlacer;
  currentDetector = tempDetector;
  currentMover = tempMover;

  Serial.begin(9600);

}

void SorticMachineSeverin::loop() {
  //delay(1000);
  bool placerHasStopped = currentPlacer->placerLoop();
  bool moverHasStopped = currentMover->moverLoop();

  //if state == idle
  if(currentMachineLogicState == MachineLogicState::idle) {
    //identify part
    currentPartColor = identifyPart(currentPart);

    //if part detected
    if (currentPartColor != partColor::none) {
      Serial.println("Detected");

      //set sorting state
      currentMachineLogicState = MachineLogicState::sorting;

      step = 0;

      //set pickup targets
      switch(currentPartColor) {
        case partColor::none:
          Serial.println("none");
          break;

        case partColor::teilSchwarz:
          Serial.println("schwarz");
          currentDropTarget = MoverPosition::dropA;
          currentPlaceDirection = PlacerActionDirection::left;
          break;

        case partColor::teilGrau:
          Serial.println("grau");
          currentDropTarget = MoverPosition::dropA;
          currentPlaceDirection = PlacerActionDirection::right;
          break;

        case partColor::teilGrauGelb:
          Serial.println("grauGelb");
          currentDropTarget = MoverPosition::dropB;
          currentPlaceDirection = PlacerActionDirection::left;
          break;

        case partColor::teilGrauSchwarz:
          Serial.println("grauSchwarz");
          currentDropTarget = MoverPosition::dropB;
          currentPlaceDirection = PlacerActionDirection::right;
          break;

        case partColor::notDeclared:
          Serial.println("not declared");
          currentDropTarget = MoverPosition::pickUp;
          currentPlaceDirection = PlacerActionDirection::right;

          //display byte array
          for(int i = 0; i<8; i++) {
            Serial.print(currentPart[i]);
            Serial.print(" ");
          }
          Serial.println("");
          break;
      }

      //set pickup targets
      currentPickupTarget = MoverPosition::pickUp;
      currentPickupDirection = PlacerActionDirection::left;
    }
  }

  //if state == sorting
  if(currentMachineLogicState == MachineLogicState::sorting) {
    if (moverHasStopped&&placerHasStopped) {
      step ++;
      //Serial.println(step);

      switch(step) {

        case 10:
        currentMover->moveToPosition(currentPickupTarget);
        break;

        case 20:
        currentPlacer->setAction(PlacerActionType::pickUp, currentPickupDirection);
        break;

        case 30:
        currentMover->moveToPosition(currentDropTarget);
        break;

        case 40:
        currentPlacer->setAction(PlacerActionType::place, currentPlaceDirection);
        break;

        case 50:
        currentMover->moveToPosition(currentPickupTarget);
        break;

        case 60:
        currentMachineLogicState = MachineLogicState::idle;
        step = 0;
        break;
      }
    }
  }

}

bool SorticMachineSeverin::arrayByte8Equals(byte a[8], byte b[8]) {
  for(int i = 0; i<7; i++)
  {
    if(a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

partColor SorticMachineSeverin::identifyPart(byte partArray[])
{
  /*
  1) is there a part?
  2) get part array
  3) identify part
  */
  if(currentDetector->RfidCardIsPresent()){
    
    currentDetector->getPartArray(currentPart);

    if(arrayByte8Equals(currentPart,teilArraySchwarz)){
      return partColor::teilSchwarz;
    }
    else if(arrayByte8Equals(currentPart,teilArrayGrau)) {
      return partColor::teilGrau;
    }
    else if(arrayByte8Equals(currentPart,teilArrayGrauGelb)) {
      return partColor::teilGrauGelb;
    }
    else if(arrayByte8Equals(currentPart,teilArrayGrauSchwarz)) {
      return partColor::teilGrauSchwarz;
    }
    else {
      return partColor::notDeclared;
    }

  } else {
    return partColor::none;
  }
}
