#ifndef SorticFramework_h
#define SorticFramework_h

#include "Arduino.h"
#include "LCLabEmbededFramework.h"

class Placer : public Component
{
  public:
    Placer();
    void pickUpLeft();
    void pickUpRight();
    void placeLeft();
    void placeRight();
    bool placerLoop(); //true = complete, false = in progress

  protected:
    bool hasPart;
    bool isMoving;
};

class Detector : public Component
{
  public:
    Detector();
    byte * getPartArray(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm

  protected:

};

enum MoverPosition {
  pickUp,
  dropA,
  dropB,
  dropC
};

class Mover : public Component
{
  public:
    Mover();
    void moveToPosition(MoverPosition newTarget);
    bool moverLoop(); //true = complete, false = in progress

  protected:
    bool movementComplete;
};

enum MachineLogicState {
  idle,
  sorting
};

class SorticMachine
{
  public:
    //Functions;
    SorticMachine(Placer *tempPlacer, Detector *tempDetector, Mover *tempMover);
    void loop();

  protected:
    Placer *currentPlacer;
    Detector *currentDetector;
    Mover *currentMover;
    MachineLogicState currentMachineLogicState;
};

#endif
