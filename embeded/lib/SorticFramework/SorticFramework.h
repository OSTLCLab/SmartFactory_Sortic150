#ifndef SorticFramework_h
#define SorticFramework_h

#include "Arduino.h"
#include "Modular.h"

enum class PlacerActionType {
  none,
  pickUp,
  place
};

enum class PlacerActionDirection {
  left,
  right,
  front
};

class Placer : public Component
{
  public:
    Placer();
    virtual void setAction(PlacerActionType newPlacerActionType, PlacerActionDirection newPlacerActionDirection);
    virtual bool placerLoop(); //true = complete, false = in progress

  protected:
    bool hasPart;
    bool isMoving;
    PlacerActionType currentPlacerActionType;
    PlacerActionDirection currentPlacerActionDirection;
};

class Detector : public Component
{
  public:
    Detector();
    virtual void getPartArray(byte partArray[]); //Function writes result into partArray[]
    //byte * getPartArray(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
    virtual bool RfidCardIsPresent();

  protected:

};

enum class MoverPosition {
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

enum class MachineLogicState {
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
    MachineLogicState currentMachineLogicState = MachineLogicState::idle;
    MoverPosition currentDropTarget;
    MoverPosition currentPickupTarget;
    PlacerActionDirection currentPickupDirection;
    PlacerActionDirection currentPlaceDirection;
};

#endif
