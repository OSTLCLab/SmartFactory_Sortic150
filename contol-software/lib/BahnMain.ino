Adafruit_DCMotor *BahnMotor = AFMS.getMotor(1);

//Median Filter Variabeln
bool BahnHasStopped;
bool ForwardIsLeft = true;
const int filterLength = 7; //Ungerade Zahl wählen!
const int numberOfFilters = 1; //Array Positionen von 0 bis numberOfFilters-1

//Position Variables
int rawSensorValue;
int filteredSensorValue;
int targetPosition;
int currentPosition;
int distanceToTarget;

int driveTollerance = 2;
int maxSpeed = 200;
bool bahnHasStopped = true;

void BahnMainSetup()
{
  rawSensorValue = analogRead(A1);
  InitializeFilter(0,rawSensorValue);
  //Motor Setup
  BahnMotor->setSpeed(0);
  BahnMotor->run(FORWARD);    //FORWARD = Nach Rechts, BACKWARD = Nach Links
  BahnMotor->run(RELEASE);
}

void BahnMainLoop()
{
  rawSensorValue = analogRead(A1);
  UpdateFilter(0,rawSensorValue);
  filteredSensorValue = getFilterValue(0);

  
  
  if((bahnHasStopped == false)and(filteredSensorValue != 0))
  {
    //Funktion fuer Uebertragung in nutzbare grösse
    int currentPosition = filteredSensorValue;
    //Serial.println(currentPosition);
    
    //In richtige Richtung fahren
    if(currentPosition>targetPosition)
    {
      //Drive left
      if(ForwardIsLeft)
      {
        BahnMotor->run(FORWARD);
      }
      else
      {
        BahnMotor->run(BACKWARD);
      }
    }
    else
    {
      //Drive right
      if(ForwardIsLeft)
      {
        BahnMotor->run(BACKWARD);
      }
      else
      {
        BahnMotor->run(FORWARD);
      }
    }

    
    distanceToTarget = abs(currentPosition-targetPosition);
    
    //Set Speed relative to target
    if(distanceToTarget > 150)
    {
      BahnMotor->setSpeed(maxSpeed);
    }
    else
    {
      BahnMotor->setSpeed(distanceToTarget+70);
    }
    
    //Stop at target
    //driveTollerance = distanceToTarget/200+1; //100->2, 200->3, 300->4, 400->5, 500->6
    if(distanceToTarget<driveTollerance)
    {
      //Serial.println(abs(currentPosition-targetPosition));
      BahnMotor->setSpeed(0);
      bahnHasStopped = true;
    }
  }
}
  
void BahnMainMoveToPosition(int newPosition)
{
  bahnHasStopped = false;
  targetPosition = newPosition;
}

bool BahnHasStoppedFunc()
{
  return bahnHasStopped;
}

void BahnSetSpeed(int speed)
{
   if (speed > 300) {
     speed = 300;
   }
   maxSpeed = speed;
}


