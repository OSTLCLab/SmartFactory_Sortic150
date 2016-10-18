//Nach Serial Event Vorlage

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

String myString = "";

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor = AFMS.getMotor(4); //Change motor number here


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  AFMS.begin();
  myMotor->setSpeed(0);
  myMotor->run(RELEASE);
  myMotor->run(FORWARD);    //FORWARD = Nach Rechts, BACKWARD = Nach Links
  delay(2000);
  //while(!Serial.available());
  Serial.println("Setup complete");
}

void loop()
{
  while (Serial.available())
  {
    int ch = Serial.read();
    if (ch != '.') // \n = Zeilenumbruch
    {
      //Serial.println("rec");
      myString += char(ch);
    }
    else
    {
      if(myString.toInt()>0)
      {
        myMotor->run(FORWARD);
        myMotor->setSpeed(myString.toInt());
        Serial.println("Moving forward");
      }
      else
      {
        myMotor->run(BACKWARD);
        myMotor->setSpeed(-myString.toInt());
        Serial.println("Moving backward");
      }
      
      // clear the string:
      myString = "";
    }
  }
}


