#ifndef config_h
#define config_h

#define DISTANCE_SENSOR A1
#define BLUETOOTH_TX 2
#define BLUETOOTH_RX 3
#define RFIDDETECTOR_SDA 7
#define RFIDDETECTOR_RST_PIN 6
#define CHASSIS_DIGITAL_TRIG_PIN 4
#define CHASSIS_DIGITAL_ECHO_PIN 5
#define MOTOR_NR 4
#define CHASIS_POS_MAX 45
#define CHASIS_POS_MIN 15
#define CHASSIS_TOLERANCE 0
#define MAX_MOTORSPEED 200
#define MOTOR_PUNISH_SPEED 10
#define MIN_MOTORSPEED 90
#define CHASSIS_POS_START 27
#define MILLIS_OF_LAST_SENDING 5000

#define RFID_LENGTH 7
#define POWER_ON "powerOn"
#define GET_STATE "getState"
#define CHASSIS_START "chassisStart"
#define RFID_SOURCE_POSITION "rfidSourcePosition"
#define HANDLING_UNIT_SLEEP_POSITION "handlingUnitInitPosition"
#define HANDLING_UNIT_ACTUAL_POSITION "actualPosition"
#define UNKNOWN_POSITION "unknownPosition"
#define ID "id"
#define DEST "dest"
#define HANDLING_UNIT "handlingUnit"

// #define DEBUG

#endif
