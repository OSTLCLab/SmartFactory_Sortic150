#ifndef SorticFramework_h
#define SorticFramework_h
/*
#include <Arduino.h>
#include <Component.h>

enum class PlacerActionType
{
  none,
  pickUp,
  place
};

enum class PlacerActionDirection
{
  left,
  right,
  front
};
template <typename T>
class Placer : public Component<T>
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

template <typename T>
class Detector : public Component<T>
{
public:
  Detector();
  virtual void getPartArray(byte partArray[]); //Function writes result into partArray[]
  //byte * getPartArray(); //Funciton returns pointer to static array: https://www.tutorialspoint.com/cplusplus/cpp_return_arrays_from_functions.htm
  virtual bool RfidCardIsPresent();
};

enum class ChassisPosition
{
  pickUp,
  dropA,
  dropB,
  dropC
};

template <typename T>
class Mover : public Component<T>
{
public:
  Mover();
  virtual void moveToPosition(ChassisPosition newTarget);
  virtual bool moverLoop(); //true = complete, false = in progress

protected:
  bool movementComplete;
};

enum class MachineJob
{
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
  MachineJob state.job = MachineJob::idle;
  ChassisPosition currentDropTarget;
  ChassisPosition currentPickupTarget;
  PlacerActionDirection currentPickupDirection;
  PlacerActionDirection currentPlaceDirection;
};*/
#endif
