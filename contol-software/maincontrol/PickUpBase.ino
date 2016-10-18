bool pickUpBaseHasStopped = true;
bool pickUpBaseForwardIsClockwise = false;

unsigned long pickUpBaseDriveSpeed = 200;
unsigned long pickUpBaseTraverseTime = 5000; //In milliseconds
unsigned long pickUpBaseStartTime;

Adafruit_DCMotor *pickUpBaseMotor  = AFMS.getMotor(2);

//Diese Werte im Hauptprogram deklarieren!
/*
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
*/

void PickUpBaseSetUp()
{
  //*pickUpBaseMotor = AFMS.getMotor(motorNumber);
  pickUpBaseMotor->setSpeed(0);
  pickUpBaseMotor->run(RELEASE);
}

void PickUpBaseLoop()  //Excecute this evey loop in the main program
{
  if(pickUpBaseHasStopped == false)
  {
    if(millis()-pickUpBaseStartTime>pickUpBaseTraverseTime)
    {
      pickUpBaseMotor->setSpeed(0);
      pickUpBaseHasStopped = true;
    }
  }
}

void PickUpBaseTurnClockwise()
{
  pickUpBaseHasStopped = false;
  pickUpBaseStartTime = millis();
  
  if(pickUpBaseForwardIsClockwise)
  {
    pickUpBaseMotor->run(FORWARD);
  }
  else
  {
    pickUpBaseMotor->run(BACKWARD);
  }
  
  pickUpBaseMotor->setSpeed(pickUpBaseDriveSpeed);
}

void PickUpBaseTurnCounterclockwise()
{
  pickUpBaseHasStopped = false;
  pickUpBaseStartTime = millis();
  
  if(pickUpBaseForwardIsClockwise)
  {
    pickUpBaseMotor->run(BACKWARD);
  }
  else
  {
    pickUpBaseMotor->run(FORWARD);
  }
  
  pickUpBaseMotor->setSpeed(pickUpBaseDriveSpeed);
}

bool pickUpBaseHasStoppedFunc()
{
  return pickUpBaseHasStopped;
}

