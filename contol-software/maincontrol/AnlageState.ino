#include <SPI.h>
#include <MFRC522.h>

MFRC522 RfidPart(6,5);
MFRC522 RfidBox(10,9);

byte teilSchwarz[8] = {4, 135, 115, 120, 162, 231, 73, 128};
byte teilGelb[8] = {4, 28, 116, 228, 162, 231, 73, 128};
byte teilGrauGelb[8] = {4, 161, 115, 94, 162, 231, 73, 128};
byte teilGrauSchwarz[8] = {4, 42, 117, 211, 162, 231, 73, 128};

#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

int programPosition = 0;
int currentTarget;
bool runComplete = true;

struct MachineState
{
  int temperature;
  int soundLevel;
  int towerPositionA;
  int towerPositionB;
  int towerPositionC;
  float towerBatteryVoltage;
  int carPosition;
  bool carError;
};

struct AmbientState
{
  int temperature;
  int soundLevel;
};

struct TowerState
{
  int towerPositionA;
  int towerPositionB;
  int towerPositionC;
  float towerBatteryVoltage;
};

AmbientState getAmbientState()
{
  AmbientState currentAmbientState;
  return currentAmbientState;
}

TowerState getTowerState()
{
  TowerState currentTowerState;
  return a;
}
void setup()
{
  Serial.begin(9600);
  //while(!Serial);   //Uncomment this line to hold the Arduino Yun until the Serial Monitor is opened.
  AFMS.begin();

  //Set up machine
  BahnMainSetup();
  PickUpBaseSetUp();
  PickUpArmSetUp();
  PickUpClawSetUp();
  RfidSetup();
  Serial.println("Setup Complete");
}

void loop()
{
  PickUpBaseLoop();
  PickUpArmLoop();
  PickUpClawLoop();
  BahnMainLoop();
  
  if(pickUpBaseHasStoppedFunc() and pickUpArmHasStoppedFunc() and pickUpClawHasStoppedFunc() and BahnHasStoppedFunc())
  {
    switch(programPosition)
    {
      case 0:
      currentTarget = RfidGetBlockTarget();
      if(currentTarget != 0)
      {
        runComplete = false;
        Serial.println("Found one");
        programPosition = 0;
      }
      break;
      
      case 1:
      Serial.print("Current Target: ");
      Serial.println(currentTarget);
      break;
      
      case 10:
      PickUpArmTurnUp();
      break;
      
      case 20:
      PickUpClawCloseAndOpen();
      //PickUpBaseTurnClockwise();
      BahnMainMoveToPosition(530);  //Aufnahmeposition
      break;
      
      case 30:
      PickUpArmTurnDown();
      break;
      
      case 40:
      PickUpClawClose();
      break;
      
      case 50:
      PickUpArmTurnUp();
      break;
      
      case 60:
      //PickUpBaseTurnCounterclockwise();
      BahnMainMoveToPosition(currentTarget);  //Ablageposition
      break;
      
      case 70:
      PickUpArmTurnDown();
      break;
      
      case 80:
      PickUpClawOpen();
      break;
      
      case 90:
      programPosition = 0;
      runComplete = true;
      
      
      /*
      Serial.println("Complete");
      while(1);
      */
      break;
    }
    if(runComplete == false)
    {
       programPosition++;
       //Serial.println(programPosition);
    }
  }
  delay(10);
}

