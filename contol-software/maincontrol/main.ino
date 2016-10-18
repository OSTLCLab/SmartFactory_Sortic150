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
bool isNorth = true; // North = PickUp side
bool opperate = false;
int maxSpeedDefault = 200;
int pickUpArmDriveSpeedDefault = 150;
int pickUpClawDriveSpeedDefault = 200;

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
  serialAPI();
  PickUpBaseLoop();
  PickUpArmLoop();
  PickUpClawLoop();
  BahnMainLoop();
  
  if(pickUpBaseHasStoppedFunc() and pickUpArmHasStoppedFunc() and pickUpClawHasStoppedFunc() and BahnHasStoppedFunc())
  {
    switch(programPosition)
    {
      case 0:
      if (opperate == true) {
        currentTarget = RfidGetBlockTarget();
        if(currentTarget != 0)
        {
          runComplete = false;
          Serial.println("Found one");
        }
      }
      break;
      
      case 1:
      Serial.print("Current Target: ");
      Serial.println(currentTarget);
      break;
      
      case 10:
      //PickUpArmTurnUp();
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
      if(currentTarget == 530)
      {
        PickUpBaseTurnClockwise();
        isNorth = false;
      }
      break;
      
      case 70:
      PickUpArmTurnDown();
      break;
      
      case 80:
      PickUpClawOpen();
      break;

      case 83:
      PickUpArmTurnUp();
      break;
      
      case 85:
      if(isNorth == false) 
      {
        PickUpBaseTurnCounterclockwise();
      }
      isNorth = true;
      BahnMainMoveToPosition(530);
      break;
      
      case 90:
      programPosition = 0;
      runComplete = true;
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

void handleSerialInstruction(String task, String value) {
   // Handle Task instructions
    if (task == "opperate") {
      opperate = (bool)value.toInt();
    }
    if (task == "speed") {
       int extra = value.toInt();
       BahnSetSpeed(maxSpeedDefault + extra);
    }
}

void serialAPI() {
  while (Serial.available() > 0) {
    //workstate:'workstate',rate:'blinkrate'\n
    //workstate:1, rate:30\n
    String msg = Serial.readStringUntil('\n');
    //workstate:'workstate',rate:'blinkrate'
    int i = -1;
    do {
      i = msg.indexOf(',');
      String order = msg.substring(0, i);
      int j = order.indexOf(':');
      String task = order.substring(0, j);
      String value = order.substring(j+1);
      task.trim();
      value.trim();
      handleSerialInstruction(task, value);
      msg = msg.substring(i + 1);      
    } while (i > 0);
  }
}

