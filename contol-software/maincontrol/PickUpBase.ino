bool pickUpBaseIsClockwise;
bool pickUpBaseHasStopped = true;
bool pickUpBaseForwardIsClockwise = false;

int pickUpBaseDriveSpeed = 200;
int pickUpBaseTraverseTime = 4500; //In milliseconds
int pickUpBaseStartTime;

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
  if((pickUpBaseIsClockwise == false)and(pickUpBaseHasStopped == true))
  {
    pickUpBaseHasStopped = false;
    pickUpBaseStartTime = millis();
    pickUpBaseIsClockwise = true;
    
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
}

void PickUpBaseTurnCounterclockwise()
{
  if((pickUpBaseIsClockwise == true)and(pickUpBaseHasStopped == true))
  {
    pickUpBaseHasStopped = false;
    pickUpBaseStartTime = millis();
    pickUpBaseIsClockwise = false;
    
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
}

bool pickUpBaseHasStoppedFunc()
{
  return pickUpBaseHasStopped;
}

