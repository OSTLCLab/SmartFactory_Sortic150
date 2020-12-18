// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x28, 0x63, 0x36, 0x7F, 0xB0, 0x31
};

const int BAUDRATE = 9600;

IPAddress Local(192, 168, 0, 16); // Local Address
IPAddress PLC(192, 168, 0, 100);  // PLC Address

// Connecting via WIFI
char ssid[] = "DigitalLab";   // Your network SSID
char pass[] = "digital42HSR";

IPAddress Gateway(192, 168, 0, 1);
IPAddress Subnet(255, 255, 255, 0);

enum jobs
{
  JOB_IDLE,
  JOB_PICKUP_BACK,
  JOB_DROP_BACK,
  JOB_PICKUP_FRONT,
  JOB_DROP_FRONT
};

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
