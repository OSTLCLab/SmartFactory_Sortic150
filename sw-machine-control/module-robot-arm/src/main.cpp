#include <Arduino.h>
/***************************************************
  Version 1.5.1
  by Simon Hersche

  Features:
  -Advanced Communication over Bluetooth (Serial1)
  -more commands implemented

   ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const float SERVOFREQ = 50; //Servo Frequenzy 50 or 60Hz

//PINS
#define MosfetPin 10 //eneable Servo
#define PiezoPin 11 //eneable Piezo
#define BoostPin 13 //eneable Booster

#define SERVOPORT0 0 //Turn Servo
#define SERVOPORT1 1 //Lift Servo
#define SERVOPORT2 2 //Grab Servo
#define FeedbackPin0 A3 //Feedback Turn Servo
#define FeedbackPin1 A2 //Feedback Lift Servo
#define FeedbackPin2 A1 //Feedback Grab Servo

//POSITIONS
#define SERVOMIN0 0   //(BACK)Turn Servo
#define SERVOMAX0 180 //(FRONT)
#define SERVOMIN1 0   //(DOWN)Lift Servo
#define SERVOMAX1 140  //(UP)
#define SERVOMIN2 0   //(CLOSED)Grab Servo
#define SERVOMAX2 180 //(OPEN)

const int Back = 0;
const int Middle = 85;  //maybe adjust?
const int Front = 180;
const int Down = 0;
const int Backup = 40;
const int Up = 140;
const int Close = 0;
const int Open = 180;

//PULSES
#define SERVOMINPULSE0 165 //(BACK)Turn Servo
#define SERVOMAXPULSE0 472 //(FRONT)
#define SERVOMINPULSE1 265 //(DOWN)Lift Servo
#define SERVOMAXPULSE1 510//(UP)
#define SERVOMINPULSE2 150 //(CLOSED)Grab Servo
#define SERVOMAXPULSE2 395 //(OPEN)

//SERVO FEEDBACK (Werte werden mit feedback_calibration angepasst!)
int SERVOMINSIGNAL0 = 270; //(BACK)Turn Servo
int SERVOMAXSIGNAL0 = 700; //(FRONT)
int SERVOMINSIGNAL1 = 270; //(DOWN)Lift Servo
int SERVOMAXSIGNAL1 = 700; //(UP)
int SERVOMINSIGNAL2 = 270; //(CLOSED)Grab Servo
int SERVOMAXSIGNAL2 = 700; //(OPEN)

//Speeds
int SPEEDSERVO0 = 5; //TURN   Time for the axis to complete
int SPEEDSERVO1 = 5; //LIFT
int SPEEDSERVO2 = 5; //GRAB

//timers
volatile unsigned long  timer_0;   //drive() Timer Axis 0
volatile unsigned long  timer_1;   //drive() Timer Axis 1
volatile unsigned long  timer_2;   //drive() Timer Axis 2

//old Positions
int oldServoPos0;
int oldServoPos1;
int oldServoPos2;

//new Positions
int  newServoPos0;
int  newServoPos1;
int  newServoPos2;

//variables
bool newmovement = true;
int Position = 99; //position on startup
bool jobDone = true;

//Serial-Control
bool flag_input_complete = false;
int i_input_string = 0;
int last_input_string = i_input_string;
String Input_String[5];

int getposdegree(int axis) {     //gets potentiometer position in degrees

  int pos;
  if (axis == 0) {
    pos = map(analogRead(FeedbackPin0), SERVOMINSIGNAL0, SERVOMAXSIGNAL0, SERVOMIN0, SERVOMAX0);
  }
  if (axis == 1) {
    pos = map(analogRead(FeedbackPin1), SERVOMINSIGNAL1, SERVOMAXSIGNAL1, SERVOMIN1, SERVOMAX1);
  }
  if (axis == 2) {
    pos = map(analogRead(FeedbackPin2), SERVOMINSIGNAL2, SERVOMAXSIGNAL2, SERVOMIN2, SERVOMAX2);
  }
  return pos;

}

int getposabsolute(int axis) {   //gets potentiometer position in "volts"

  int pos;
  if (axis == 0) {
    pos = analogRead(FeedbackPin0);
  }
  if (axis == 1) {
    pos = analogRead(FeedbackPin1);
  }
  if (axis == 2) {
    pos = analogRead(FeedbackPin2);
  }
  return pos;
}

int go(int pos0, int pos1, int pos2) {    //drives with full speed
  pwm.setPWM(SERVOPORT0, 0, map(pos0, SERVOMIN0, SERVOMAX0, SERVOMINPULSE0, SERVOMAXPULSE0));
  pwm.setPWM(SERVOPORT1, 0, map(pos1, SERVOMIN1, SERVOMAX1, SERVOMINPULSE1, SERVOMAXPULSE1));
  pwm.setPWM(SERVOPORT2, 0, map(pos2, SERVOMIN2, SERVOMAX2, SERVOMINPULSE2, SERVOMAXPULSE2));
}

void feedback_calibration() {

  go(SERVOMAX0, SERVOMAX1, SERVOMAX2);
  delay(1500);
  SERVOMAXSIGNAL0 = getposabsolute(0);
  SERVOMAXSIGNAL1 = getposabsolute(1);
  SERVOMAXSIGNAL2 = getposabsolute(2);

  go(SERVOMIN0, SERVOMIN1, SERVOMIN2);
  delay(1500);
  SERVOMINSIGNAL0 = getposabsolute(0);
  SERVOMINSIGNAL1 = getposabsolute(1);
  SERVOMINSIGNAL2 = getposabsolute(2);

  oldServoPos0 = getposdegree(0);
  oldServoPos1 = getposdegree(1);
  oldServoPos2 = getposdegree(2);

  Position = 5;
}

int drive(int pos0, int pos1, int pos2) {   //drives with predefined speed

  newServoPos0 = pos0;
  newServoPos1 = pos1;
  newServoPos2 = pos2;

  int angle0 = oldServoPos0;
  int angle1 = oldServoPos1;
  int angle2 = oldServoPos2;

  //Serial.print("Start pos0: "); Serial.println(oldServoPos0);
  //Serial.print("End pos0: "); Serial.println(newServoPos0);

  int movetime;
  int delaytime_0 = map(SPEEDSERVO0, 10, 1, 1, 20);  //"delaytimes" from 1-20ms per step
  int delaytime_1 = map(SPEEDSERVO1, 10, 1, 1, 20);
  int delaytime_2 = map(SPEEDSERVO2, 10, 1, 1, 20);

  int moveangle0 = oldServoPos0 - newServoPos0;
  int moveangle1 = oldServoPos1 - newServoPos1;
  int moveangle2 = oldServoPos2 - newServoPos2;

  movetime = max(abs(moveangle0) * delaytime_0, abs(moveangle1) * delaytime_1);
  movetime = max(movetime, abs(moveangle2) * delaytime_2);

  int steptime_0 = movetime / abs(moveangle0);
  int steptime_1 = movetime / abs(moveangle1);
  int steptime_2 = movetime / abs(moveangle2);

  const int stepangle = 1;
  /*
    Serial.print("movetime "); Serial.println(movetime);
    Serial.print("steptime_0 "); Serial.println(steptime_0);
    Serial.print("steptime_1 "); Serial.println(steptime_1);
    Serial.print("steptime_2 "); Serial.println(steptime_2);
  */
  if (newServoPos0 >= SERVOMIN0 && newServoPos0 <= SERVOMAX0) {              //check if target value is in range
    if (oldServoPos0 < newServoPos0 && millis() - timer_0 >= steptime_0) {
      angle0 += stepangle;
      pwm.setPWM(SERVOPORT0, 0, map(angle0, SERVOMIN0, SERVOMAX0, SERVOMINPULSE0, SERVOMAXPULSE0));
      //Serial.print("SERVO0+"); Serial.println(angle0);
      oldServoPos0 = angle0;
      timer_0 = millis();
    }
    if (oldServoPos0 > newServoPos0 && millis() - timer_0 >= steptime_0) {
      angle0 -= stepangle;
      pwm.setPWM(SERVOPORT0, 0, map(angle0, SERVOMIN0, SERVOMAX0, SERVOMINPULSE0, SERVOMAXPULSE0));
      //Serial.print("SERVO0-"); Serial.println(angle0);
      oldServoPos0 = angle0;
      timer_0 = millis();
    }
  }

  if (newServoPos1 >= SERVOMIN1 && newServoPos1 <= SERVOMAX1) {
    if (oldServoPos1 < newServoPos1 && millis() - timer_1 >= steptime_1) {
      angle1 += stepangle;
      pwm.setPWM(SERVOPORT1, 0, map(angle1, SERVOMIN1, SERVOMAX1, SERVOMINPULSE1, SERVOMAXPULSE1));
      //Serial.print("SERVO1+"); Serial.println(angle1);
      oldServoPos1 = angle1;
      timer_1 = millis();
    }
    if (oldServoPos1 > newServoPos1 && millis() - timer_1 >= steptime_1) {
      angle1 -= stepangle;
      pwm.setPWM(SERVOPORT1, 0, map(angle1, SERVOMIN1, SERVOMAX1, SERVOMINPULSE1, SERVOMAXPULSE1));
      //Serial.print("SERVO1-"); Serial.println(angle1);
      oldServoPos1 = angle1;
      timer_1 = millis();
    }
  }

  if (newServoPos2 >= SERVOMIN2 && newServoPos2 <= SERVOMAX2) {
    if (oldServoPos2 < newServoPos2 && millis() - timer_2 >= steptime_2) {
      angle2 += stepangle;
      pwm.setPWM(SERVOPORT2, 0, map(angle2, SERVOMIN2, SERVOMAX2, SERVOMINPULSE2, SERVOMAXPULSE2));
      //Serial.print("Servo2+"); Serial.println(angle2);
      oldServoPos2 = angle2;
      timer_2 = millis();
    }
    if (oldServoPos2 > newServoPos2 && millis() - timer_2 >= steptime_2) {
      angle2 -= stepangle;
      pwm.setPWM(SERVOPORT2, 0, map(angle2, SERVOMIN2, SERVOMAX2, SERVOMINPULSE2, SERVOMAXPULSE2));
      //Serial.print("Servo2-"); Serial.println(angle2);
      oldServoPos2 = angle2;
      timer_2 = millis();
    }
  }
}

void position_handling() {
  switch (Position) {
     case 0:  //no movement
       break;
     case 1:
       drive(Front, Backup, Open);
       break;
     case 2:
       drive(Front, Down, Open);
       break;
     case 3:
       drive(Front, Down, Close);
       break;
     case 4:
       drive(Front, Backup, Close);
       break;
     case 5:
       drive(Middle, Up, Close);
       break;
     case 6:
       drive(Back, Backup, Close);
       break;
     case 7:
       drive(Back, Down, Close);
       break;
     case 8:
       drive(Back, Down, Open);
       break;
     case 9:
       drive(Back, Backup, Open);
       break;
   }
}

void battery_management() {
  //ToDo Defin Pin from Battery as Config
  float vbat = analogRead(9) * 2 * 3.3 / 1024;   //get Battery-Voltage
  if (vbat < 3.4) {
    tone(PiezoPin, 500, 200);
  }
}

bool moving() {
  int tolerance = 5;
  if ((newServoPos0-oldServoPos0)<tolerance && (newServoPos1-oldServoPos1<tolerance) && (newServoPos2-oldServoPos2)<tolerance) {
    return false;
  }
  else {
    return true;
  }
}

void pickup() {

}

void drop() {

}

void serial1_Control() {

    if (Input_String[0] == "gotoPosition" && flag_input_complete == true) {  //go to Position
      int newPos = Input_String[1].toInt();
      Position = newPos;
      jobDone = false;
    }

    if (Input_String[0] == "getPosition" && flag_input_complete == true) {  //get Position
      Serial1.print("Position(");
      Serial1.print(Position);
      Serial1.println(")");
    }

    if (Input_String[0] == "getBatteryState" && flag_input_complete == true) {   //BatteryState
      float vbat = analogRead(9) * 2 * 3.3 / 1024;
      Serial1.print("Batteryvoltage(" );
      Serial1.print(vbat);
      Serial1.println(")");
    }

    if (Input_String[0] == "setMotors" && flag_input_complete == true) {    //turn Motors off/on
      if (Input_String[1] == "1") {
        digitalWrite(MosfetPin, LOW);  //turn Mosfet on
        Serial1.println("Motorstate(1)");
      }
      if (Input_String[1] == "0") {
        digitalWrite(MosfetPin, HIGH); //turn Mosfet off
        Serial1.println("Motorstate(0)");
      }
    }

    if (Input_String[0] == "setSpeed" && flag_input_complete == true) {       //set Speed
      int newspeed = Input_String[1].toInt();
      if (newspeed <= 10) {
        SPEEDSERVO0 = newspeed;
        SPEEDSERVO1 = newspeed;
        SPEEDSERVO2 = newspeed;
        Serial1.print("newSpeed(");
        Serial1.print(newspeed);
        Serial1.println(")");
      }
    }

  if (Input_String[0] == "getAngles" && flag_input_complete == true) {       //get Angles of Servos

    //shows the Position at the moment
    int pos0 = getposdegree(0);
    int pos1 = getposdegree(1);
    int pos2 = getposdegree(2);
    Serial1.print("Angles(");
    Serial1.print(pos0); Serial1.print(",");
    Serial1.print(pos1); Serial1.print(",");
    Serial1.print(pos2); Serial1.print(",");
    Serial1.println(")");
  }

  if (Input_String[0] == "getState" && flag_input_complete == true) {       //get State
    Serial1.print("State(");
    Serial1.print(moving());
    Serial1.println(")");
  }

  if (Input_String[0] == "FeedbackCalibration" && flag_input_complete == true) {  //go to Position
    feedback_calibration();   //calibration is necessary because of voltage-drop of the battery
  }

  if (flag_input_complete) {

    bool debug = false;
    if (debug) { //only for debugging
      //Serial1.print("Input_String[0]: ");
      Serial1.println(Input_String[0]);
      //Serial1.print("Input_String[1]: ");
      Serial1.println(Input_String[1]);
      //Serial1.print("Input_String[2]: ");
      Serial1.println(Input_String[2]);
      //Serial1.print("Input_String[3]: ");
      Serial1.println(Input_String[3]);
      //Serial1.print("Input_String[4]: ");
      Serial1.println(Input_String[4]);
    }

    Input_String[0] = "";
    Input_String[1] = "";
    Input_String[2] = "";
    Input_String[3] = "";
    Input_String[4] = "";
    i_input_string = 0;
    flag_input_complete = false;
  }
}

void serial1Event() {
  if (Serial1.available() > 0 && flag_input_complete != true) {
    char inChar = (char)Serial1.read();
    if (inChar == '(' || inChar == ')' || inChar == '\n' || inChar == ',') {
      last_input_string = i_input_string;
      i_input_string = 4;
    }
    Input_String[i_input_string] += inChar;
    if (i_input_string == 4) {
      i_input_string = last_input_string + 1;
    }
    if (inChar == '\n') {
      flag_input_complete = true;
    }
  }
  serial1_Control();
}


void setup() {

  pinMode(BoostPin, OUTPUT);
  pinMode(MosfetPin, OUTPUT);
  pinMode(PiezoPin, OUTPUT);

  digitalWrite(BoostPin, HIGH); //Turn off Booster
  digitalWrite(MosfetPin, LOW); //Turn on Motors
  digitalWrite(PiezoPin, LOW);  //Turn off Piezo

  delay(1000);

  Serial1.begin(57600);  //set Bluetooth-baudrate correctly!
  Serial1.println("Feather is ready!");

  pwm.begin();
  pwm.setPWMFreq(SERVOFREQ);

  feedback_calibration();   //calibration is necessary because of voltage-differences of the battery
  yield();
}

void loop() {
  serial1Event();
  //position_handling();
  battery_management();
  if (moving() == false && jobDone == false) {
    Serial1.print("arrivedPosition(");
    Serial1.print(Position);
    Serial1.println(")");
    jobDone = true;
  }
}
