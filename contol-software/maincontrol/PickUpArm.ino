bool pickUpArmIsUp;
bool pickUpArmHasStopped = true;
bool pickUpArmForwardIsUp = false;

int pickUpArmDriveSpeed = 150;
unsigned long pickUpArmTraverseTimeCurrent; //Either up or down depending on the movement direction
unsigned long pickUpArmTraverseTimeUp = 2100; //In milliseconds
unsigned long pickUpArmTraverseTimeDown = 2800; //In milliseconds
unsigned long pickUpArmStartTime;

Adafruit_DCMotor *pickUpArmMotor  = AFMS.getMotor(3);

void PickUpArmSetUp()
{
  //*pickUpArmMotor = AFMS.getMotor(motorNumber);
  pickUpArmMotor->setSpeed(0);
  pickUpArmMotor->run(RELEASE);
}

void PickUpArmLoop()  //Excecute this evey loop in the main program
{
  if(pickUpArmHasStopped==false)
  {
    if(millis()-pickUpArmStartTime>pickUpArmTraverseTimeCurrent)
    {
      pickUpArmMotor->setSpeed(0);
      pickUpArmHasStopped = true;
    }
    //Serial.println(millis()-pickUpArmStartTime);
  }
}

void PickUpArmTurnUp()
{
  pickUpArmHasStopped = false;
  pickUpArmStartTime = millis();
  pickUpArmIsUp = true;
  pickUpArmTraverseTimeCurrent = pickUpArmTraverseTimeUp;
  
  if(pickUpArmForwardIsUp)
  {
    pickUpArmMotor->run(FORWARD);
  }
  else
  {
    pickUpArmMotor->run(BACKWARD);
  }
  
  pickUpArmMotor->setSpeed(pickUpArmDriveSpeed);
}

void PickUpArmTurnDown()
{
  pickUpArmHasStopped = false;
  pickUpArmStartTime = millis();
  pickUpArmIsUp = false;
  pickUpArmTraverseTimeCurrent = pickUpArmTraverseTimeDown;
  
  if(pickUpArmForwardIsUp)
  {
    pickUpArmMotor->run(BACKWARD);
  }
  else
  {
    pickUpArmMotor->run(FORWARD);
  }
  
  pickUpArmMotor->setSpeed(pickUpArmDriveSpeed);
}

bool pickUpArmHasStoppedFunc()
{
  return pickUpArmHasStopped;
}

