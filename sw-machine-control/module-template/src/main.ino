#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include <Servo.h>

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

enum jobs {
  JOB_IDLE,
  JOB_PICKUP,
  JOB_DROP
};

int moduleJob;
Servo myservo;
int pos = 0;
int direction = 1;

void setup() {
  while (!Serial); { // required for Flora & Micro
    delay(500);
  }

  Serial.begin(9600);
  initBLE();
  ble.verbose(false);  // debug info is a little annoying after this point!
  /* Wait for connection
  while (! ble.isConnected()) {
      delay(500);
  }*/

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  moduleJob = JOB_IDLE;
  myservo.write(90);
}


void loop() {
  listenBLE();
  if (moduleJob != JOB_IDLE) {
    if (moduleJob == JOB_DROP) {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      pos++;
      if (pos > 180) {
        moduleJob = JOB_IDLE;
        sendBLE("success(1)");
        pos = 180;
      }
    }
    if (moduleJob == JOB_PICKUP) {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      pos--;
      if (pos < 0) {
        moduleJob = JOB_IDLE;
        sendBLE("success(1)");
        pos = 0;
      }
    }
  }
  delay(10);
}

// Handle API commands
void handleApiCommands(String command) {
  if (command == "pickup();") {
    moduleJob = JOB_PICKUP;
  }
  if (command == "drop();") {
    moduleJob = JOB_DROP;
  }
}

// Listen to incomminc commands from Bluetooth
void listenBLE() {
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }
  handleApiCommands(ble.buffer);
}

// Send message over Bluetooth
void sendBLE(String msg) {
  ble.print("AT+BLEUARTTX=");
  ble.println(msg);

  // check response stastus
  if (! ble.waitForOK() ) {
    Serial.println(F("Failed to send?"));
  }
}

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void initBLE() {
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();
}
