#ifndef SorticFramework_h
#define SorticFramework_h

#include "Arduino.h"
#include "Modular.h"

class Placer : public Component
{
  public:
    Placer();

    virtual void pickUpLeft();
    virtual void pickUpRight();
    virtual void placeLeft();
    virtual void placeRight();
    virtual bool placerLoop(); //true = complete, false = in progress

  protected:
    bool hasPart;
    bool isMoving;
};

class Detector : public Component
{
  public:
    Detector();
    void getPartArray(byte partArray[]); //Function writes result into partArray[]
    //byte * getPartArray(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
    bool RfidCardIsPresent();

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
    virtual void moveToPosition(MoverPosition newTarget);
    virtual bool moverLoop(); //true = complete, false = in progress

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
    virtual void loop();

  protected:
    Placer *currentPlacer;
    Detector *currentDetector;
    Mover *currentMover;
    MachineLogicState currentMachineLogicState;
};

#endif
