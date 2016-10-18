bool pickUpClawIsOpen = true;
bool pickUpClawHasStopped = true;
bool pickUpClawForwardIsOpen = false;
bool pickUpClawNeedsToOpenAgain = false;

int pickUpClawDriveSpeed = 200;
unsigned long pickUpClawTraverseTimeCurrent; //In milliseconds
unsigned long pickUpClawTraverseTimeOpen = 900;
unsigned long pickUpClawTraverseTimeClose = 1500;
unsigned long pickUpClawStartTime;

Adafruit_DCMotor *pickUpClawMotor  = AFMS.getMotor(4);

//Diese Werte im Hauptprogram deklarieren!
/*
  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
*/

void PickUpClawSetUp()
{
  pickUpClawMotor->setSpeed(0);
  pickUpClawMotor->run(RELEASE);
}

void PickUpClawLoop()  //Excecute this evey loop in the main program
{
  if(pickUpClawHasStopped == false)
  {
    if(millis()-pickUpClawStartTime>pickUpClawTraverseTimeCurrent)
    {
      if(pickUpClawNeedsToOpenAgain == false)
      {
        pickUpClawMotor->setSpeed(0);
        pickUpClawHasStopped = true;
      }
      else
      {
        pickUpClawNeedsToOpenAgain = false;
        pickUpClawStartTime = millis();
        pickUpClawIsOpen = true;
        pickUpClawMotor->setSpeed(0);
        if(pickUpClawForwardIsOpen)
        {
          pickUpClawMotor->run(FORWARD);
        }
        else
        {
          pickUpClawMotor->run(BACKWARD);
        }
        pickUpClawTraverseTimeCurrent = pickUpClawTraverseTimeOpen*2;
        pickUpClawMotor->setSpeed(pickUpClawDriveSpeed);
      }
    }
  }
}

void PickUpClawOpen()
{
  if((pickUpClawIsOpen == false)and(pickUpClawHasStopped == true))
  {
    pickUpClawHasStopped = false;
    pickUpClawStartTime = millis();
    pickUpClawIsOpen = true;
    pickUpClawTraverseTimeCurrent = pickUpClawTraverseTimeOpen;
    
    if(pickUpClawForwardIsOpen)
    {
      pickUpClawMotor->run(FORWARD);
    }
    else
    {
      pickUpClawMotor->run(BACKWARD);
    }
    
    pickUpClawMotor->setSpeed(pickUpClawDriveSpeed);
  }
}

void PickUpClawClose()
{
  if((pickUpClawIsOpen == true)and(pickUpClawHasStopped == true))
  {
    pickUpClawHasStopped = false;
    pickUpClawStartTime = millis();
    pickUpClawIsOpen = false;
    pickUpClawTraverseTimeCurrent = pickUpClawTraverseTimeClose;
    
    if(pickUpClawForwardIsOpen)
    {
      pickUpClawMotor->run(BACKWARD);
    }
    else
    {
      pickUpClawMotor->run(FORWARD);
    }
    
    pickUpClawMotor->setSpeed(pickUpClawDriveSpeed);
  }
}

void PickUpClawCloseAndOpen()
{
  if(pickUpClawHasStopped == true)
  {
    pickUpClawHasStopped = false;
    pickUpClawNeedsToOpenAgain = true;
    pickUpClawStartTime = millis();
    pickUpClawTraverseTimeCurrent = pickUpClawTraverseTimeClose;
    
    if(pickUpClawForwardIsOpen)
    {
      pickUpClawMotor->run(BACKWARD);
    }
    else
    {
      pickUpClawMotor->run(FORWARD);
    }
    
    pickUpClawMotor->setSpeed(pickUpClawDriveSpeed);
  }
}

bool pickUpClawHasStoppedFunc()
{
  return pickUpClawHasStopped;
}

