#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//#define BLUETOOTH
//Servo Ports
#define SERVO_TURN 4    //Turn Servo
#define SERVO_LIFT 5    //Lift Servo
#define SERVO_GRAB 6    //Grab Servo
//PULSES
#define TURN_BACK 165 //(BACK)Turn Servo
#define TURN_MIDDLE 330 //(FRONT) ...
#define TURN_FRONT 505 //(FRONT) ...
#define LIFT_DOWN_PICKUP 340 //(DOWN)Lift Servo
#define LIFT_DOWN_DROP 390 //(DOWN)Lift Servo
#define LIFT_UP 510 //(UP)
#define GRAB_CLOSE 380 //150 //(CLOSED)Grab Servo
#define GRAB_OPEN 50 //395 //(OPEN)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int posTurn = TURN_MIDDLE;
int posLift = LIFT_UP;
int posGrab = GRAB_CLOSE;

int speed = 10;

enum jobs {
  JOB_IDLE,
  JOB_PICKUP_BACK,
  JOB_DROP_BACK,
  JOB_PICKUP_FRONT,
  JOB_DROP_FRONT
};

int moduleJob;

int moveServo(int servo, int from, int to, int s) {
  if (to > from) {
    for (uint16_t pulselen = from; pulselen < to; pulselen++) {
      pwm.setPWM(servo, 0, pulselen);
      delay(s);
    }
  } else {
     for (uint16_t pulselen = from; pulselen > to; pulselen--) {
      pwm.setPWM(servo, 0, pulselen);
      delay(s);
    }
  }
  return to;
}

int moveServo(int servo, int from, int to) {
  return moveServo(servo, from, to, speed);
}

void releaseServos() {
  pwm.setPWM(SERVO_TURN, 0, 0);
  pwm.setPWM(SERVO_GRAB, 0, 0);
  pwm.setPWM(SERVO_LIFT, 0, 0);
}

void setup() {
  initBT();
  pwm = Adafruit_PWMServoDriver(0x40);
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
void loop() {
  listenBT();
  if (moduleJob != JOB_IDLE) {
    if (moduleJob == JOB_DROP_BACK || moduleJob == JOB_DROP_FRONT) {
      if (moduleJob == JOB_DROP_BACK) {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      } else {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_FRONT);
      }
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN_DROP);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos();
      moduleJob = JOB_IDLE;
      sendBT("success(1)");
    }
    if (moduleJob == JOB_PICKUP_BACK || moduleJob == JOB_PICKUP_FRONT) {
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      if (moduleJob == JOB_PICKUP_BACK) {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      } else {
        posTurn = moveServo(SERVO_TURN, posTurn, TURN_FRONT);
      }
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN_PICKUP);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_CLOSE);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos();
      moduleJob = JOB_IDLE;
      sendBT("success(1)");
    }
  }
  //sendBT(".");
  delay(100);
}

// Handle API commands
void handleApiCommands(String command) {
  if (command.startsWith("pickup(0)")) {
    moduleJob = JOB_PICKUP_BACK;
  }
  if (command.startsWith("pickup(1)")) {
    moduleJob = JOB_PICKUP_FRONT;
  }
  if (command.startsWith("drop(0)")) {
    moduleJob = JOB_DROP_BACK;
  }
  if (command.startsWith("drop(1)")) {
    moduleJob = JOB_DROP_FRONT;
  }
}

// Listen to incomminc commands from BLUETOOTH
void listenBT() {
  String buffer = "";
  #ifdef BLUETOOTH
    if (Serial1.available()) {
      buffer = Serial1.readStringUntil('\n');
    }
  #else
    if (Serial.available()) {
      buffer = Serial.readStringUntil('\n');
    }
  #endif
  if (buffer != "") {
    handleApiCommands(buffer);
  }
}

// Send message over BLUETOOTH
void sendBT(String msg) {
  #ifdef BLUETOOTH
    Serial1.println(msg);
  #else
    Serial.println(msg);
  #endif
}

// A small helper
void error(const __FlashStringHelper*err) {
  #ifdef BLUETOOTH
    Serial1.println(err);
  #else
    Serial.println(err);
  #endif
}

void initBT() {
  #ifdef BLUETOOTH
    Serial1.begin(9600);
  #else
    Serial.begin(9600);
    while (!Serial) {
      delay(1);
    }
  #endif
  sendBT("ready");
}
