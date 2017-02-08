#include "SorticFramework.h"

//Constructor

SorticMachine::SorticMachine(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover) {
  currentPlacer = tempPlacer;
  currentDetector = tempDetector;
  currentMover = tempMover;
  currentMachineLogicState = MachineLogicState::idle;
}


Placer::Placer() {

}

Detector::Detector() {

}

Mover::Mover() {

}
