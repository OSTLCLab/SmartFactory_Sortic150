#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//#define BLUETOOTH
#define SERIAL_TX 1
#define SERIAL_RX 0

//Servo Ports
#define SERVO_TURN 0    //Turn Servo
#define SERVO_LIFT 1    //Lift Servo
#define SERVO_GRAB 2    //Grab Servo
//PULSES
#define TURN_BACK 165 //(BACK)Turn Servo
#define TURN_MIDDLE 310 //(FRONT)
#define TURN_FRONT 472 //(FRONT)
#define LIFT_DOWN 265 //(DOWN)Lift Servo
#define LIFT_UP 510 //(UP)
#define GRAB_CLOSE 150 //(CLOSED)Grab Servo
#define GRAB_OPEN 395 //(OPEN)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int posTurn = TURN_MIDDLE;
int posLift = LIFT_UP;
int posGrab = GRAB_CLOSE;

int speed = 10;

enum jobs {
  JOB_IDLE,
  JOB_PICKUP,
  JOB_DROP
};

int moduleJob;
//static SoftwareSerial1 Serial1{Serial1_TX, Serial1_RX};
HardwareSerial bluetooth = Serial1;


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
  releaseServos;
  moduleJob = JOB_IDLE;
}

// Stupid, simple implemetation
void loop() {
  listenBT();
  if (moduleJob != JOB_IDLE) {
    if (moduleJob == JOB_DROP) {
      //Implement Left or Right;
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos;
      moduleJob = JOB_IDLE;
      sendBT("success(1)");
    }
    if (moduleJob == JOB_PICKUP) {
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_OPEN);
      //Implement Left or Right;
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_BACK);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_DOWN);
      posGrab = moveServo(SERVO_GRAB, posGrab, GRAB_CLOSE);
      posLift = moveServo(SERVO_LIFT, posLift, LIFT_UP);
      posTurn = moveServo(SERVO_TURN, posTurn, TURN_MIDDLE);
      releaseServos;
      moduleJob = JOB_IDLE;
      sendBT("success(1)");
    }
  }
  //sendBT(".");
  delay(100);
}

// Handle API commands
void handleApiCommands(String command) {
  //sendBT(command);
  if (command.startsWith("pickup(")) {
    moduleJob = JOB_PICKUP;
  }
  if (command.startsWith("drop(")) {
    moduleJob = JOB_DROP;
  }
}

// Listen to incomminc commands from bluetooth
void listenBT() {
  String buffer = "";
  #ifdef BLUETOOTH
    if (bluetooth.available()) {
      buffer = bluetooth.readStringUntil('\n');
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

// Send message over bluetooth
void sendBT(String msg) {
  #ifdef BLUETOOTH
    bluetooth.println(msg);
  #else
    Serial.println(msg);
  #endif
}

// A small helper
void error(const __FlashStringHelper*err) {
  #ifdef BLUETOOTH
    bluetooth.println(err);
  #else
    Serial.println(err);
  #endif
}

void initBT() {
  #ifdef BLUETOOTH
    bluetooth.begin(57600);
  #else
    Serial.begin(9600);
    while (!Serial) {
      delay(1);
    }
  #endif
  sendBT("bluetooth connected and ready");
}
