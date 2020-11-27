#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Platform.h"
#include "Settimino.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x28, 0x63, 0x36, 0x7F, 0xB0, 0x31
};

IPAddress Local(192, 168, 0, 16); // Local Address
IPAddress PLC(192, 168, 0, 100);  // PLC Address

// Connecting via WIFI
char ssid[] = "WG61";   // Your network SSID
char pass[] = "C0meinandfind0ut";

int status = WL_IDLE_STATUS;     // the WiFi radio's status
IPAddress Gateway(192, 168, 0, 1);
IPAddress Subnet(255, 255, 255, 0);

int DBNum = 100; // This DB must be present in your PLC
byte Buffer[1024];

S7Client Client;
S7Helper Helper;

unsigned long Elapsed; // To calc the execution time

//Servo Ports
#define SERVO_TURN 0 //Turn Servo
#define SERVO_LIFT 1 //Lift Servo
#define SERVO_GRAB 2 //Grab Servo
//PULSES
#define TURN_BACK 165        //(BACK)Turn Servo
#define TURN_MIDDLE 330      //(FRONT) ...
#define TURN_FRONT 505       //(FRONT) ...
#define LIFT_DOWN_PICKUP 362 //(DOWN)Lift Servo
#define LIFT_DOWN_DROP 300   //(DOWN)Lift Servo
#define LIFT_UP 505          //(UP)
#define GRAB_CLOSE 380       //150 //(CLOSED)Grab Servo
#define GRAB_OPEN 50         //395 //(OPEN)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int posTurn = TURN_MIDDLE;
int posLift = LIFT_UP;
int posGrab = GRAB_CLOSE;

int speed = 10;

enum jobs
{
  JOB_IDLE,
  JOB_PICKUP_BACK,
  JOB_DROP_BACK,
  JOB_PICKUP_FRONT,
  JOB_DROP_FRONT
};

int moduleJob;

int moveServo(int servo, int from, int to, int s)
{
  if (to > from)
  {
    for (uint16_t pulselen = from; pulselen < to; pulselen++)
    {
      pwm.setPWM(servo, 0, pulselen);
      delay(s);
    }
  }
  else
  {
    for (uint16_t pulselen = from; pulselen > to; pulselen--)
    {
      pwm.setPWM(servo, 0, pulselen);
      delay(s);
    }
  }
  return to;
}

int moveServo(int servo, int from, int to)
{
  return moveServo(servo, from, to, speed);
}

void releaseServos()
{
  pwm.setPWM(SERVO_TURN, 0, 0);
  pwm.setPWM(SERVO_GRAB, 0, 0);
  pwm.setPWM(SERVO_LIFT, 0, 0);
}

void setup()
{

  // feather M0 ATWINC1500
  WiFi.setPins(8, 7, 4, 2);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
/**  while (!Serial)
  {
    delay(1);
  }
*/
  //--------------------------------------------- ESP8266 Initialization
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  //WiFi.config(Local, Gateway, Subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP address : ");
  Serial.println(WiFi.localIP());

  pwm.begin();
  pwm.setPWMFreq(50);
  pwm.setPWM(SERVO_TURN, 0, posTurn);
  pwm.setPWM(SERVO_LIFT, 0, posLift);
  pwm.setPWM(SERVO_GRAB, 0, posGrab);

  posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_CLOSE);
  posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
  releaseServos();
  moduleJob = JOB_IDLE;
}

// Stupid, most simple implemetation
void loop()
{
  listen();

  if (moduleJob != JOB_IDLE)
  {
    if (moduleJob == JOB_DROP_BACK || moduleJob == JOB_DROP_FRONT)
    {
      if (moduleJob == JOB_DROP_BACK)
      {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      }
      else
      {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_FRONT);
      }
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN_DROP);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos();
      moduleJob = JOB_IDLE;

      Serial.println("success(1)");
      //Helper.SetFloatAt(Buffer, 2, 1.0); // Position nach Bool im Buffer
      Helper.SetBitAt(Buffer, 0, 0, true); // Position 0 im DB100 SPS
    }
    if (moduleJob == JOB_PICKUP_BACK || moduleJob == JOB_PICKUP_FRONT)
    {
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      if (moduleJob == JOB_PICKUP_BACK)
      {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      }
      else
      {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_FRONT);
      }
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN_PICKUP);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_CLOSE);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos();
      moduleJob = JOB_IDLE;

      Serial.println("success(1)");
      //Helper.SetFloatAt(Buffer, 2, 1.0); // Position nach Bool im Buffer
      Helper.SetBitAt(Buffer, 0, 0, true); // Position 0 im DB100 SPS
    }
  }

  delay(100);
}

// Listen to incomming commands
void listen()
{
  int Size, Result;
  void *Target;

  Size = 1024;
  Target = &Buffer; // Uses a larger buffer

  // Connection
  while (!Client.Connected)
  {
    if (!Connect())
      delay(500);
  }

  Serial.print("Reading ");
  Serial.print(Size);
  Serial.print(" bytes from DB");
  Serial.println(DBNum);
  // Get the current tick
  MarkTime();
  Result = Client.ReadArea(S7AreaDB, // We are requesting DB access
                           DBNum,    // DB Number
                           0,        // Start from byte N.0
                           Size,     // We need "Size" bytes
                           Target);  // Put them into our target (Buffer or PDU)
  if (Result == 0)
  {
    ShowTime();

    // print bool and float
    //bool done = Helper.BitAt(Target, 0, 0);
    //Serial.print("done = ");
    //Serial.println(isTrue);

    int comPos = Helper.FloatAt(Target, 2);
    Serial.print("sollPos = ");
    Serial.println(comPos);

    if (comPos == 0) { //idle
    }
    if (comPos == 1) {
      moduleJob = JOB_PICKUP_BACK;
    }
    if (comPos == 2) {
      moduleJob = JOB_PICKUP_FRONT;
    }
    if (comPos == 3) {
      moduleJob = JOB_DROP_BACK;
    }
    if (comPos == 4) {
      moduleJob = JOB_DROP_FRONT;
    }
    if (comPos > 4) {
      Serial.println("error OPC: comPos exeeds 4");
    }
  }
  else
    CheckError(Result);

  delay(500);

  if (Helper.BitAt(Target, 0, 0)) {
    // WRITE (changed data in function dump() )
    Serial.print("Writing ");
    Serial.print(Size);
    Serial.print(" bytes into DB");
    Serial.println(DBNum);
    // Get the current tick
    MarkTime();
    Result = Client.WriteArea(S7AreaDB, // We are requesting DB access
                              DBNum,    // DB Number
                              0,        // Start from byte N.0
                              Size,     // We need "Size" bytes
                              Target);  // Pointer to Data
    if (Result == 0)
    {
      ShowTime();
    }
    else
      CheckError(Result);

    delay(500);
  }
}

// A small helper
void error(const __FlashStringHelper *err)
{
  Serial.println(err);
}

bool Connect()
{
  int Result = Client.ConnectTo(PLC,
                                0,  // Rack (see the doc.)
                                0); // Slot (see the doc.)
  Serial.print("Connecting to ");
  Serial.println(PLC);
  if (Result == 0)
  {
    Serial.print("Connected ! PDU Length = ");
    Serial.println(Client.GetPDULength());
  }
  else
    Serial.println("Connection error");
  return Result == 0;
}

//----------------------------------------------------------------------
// Prints the Error number
//----------------------------------------------------------------------
void CheckError(int ErrNo)
{
  Serial.print("Error No. 0x");
  Serial.println(ErrNo, HEX);

  // Checks if it's a Severe Error => we need to disconnect
  if (ErrNo & 0x00FF)
  {
    Serial.println("SEVERE ERROR, disconnecting.");
    Client.Disconnect();
  }
}
//----------------------------------------------------------------------
// Profiling routines
//----------------------------------------------------------------------
void MarkTime()
{
  Elapsed = millis();
}
//----------------------------------------------------------------------
void ShowTime()
{
  // Calcs the time
  Elapsed = millis() - Elapsed;
  Serial.print("Job time (ms) : ");
  Serial.println(Elapsed);
}