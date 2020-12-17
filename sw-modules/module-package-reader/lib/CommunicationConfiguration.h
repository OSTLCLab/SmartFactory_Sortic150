/**
 * @file CommunicationConfiguration.h
 * @brief file that contains the default communication configuration
 * 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.1
 * 
 * @date 2012-12-07
 * 
 * @copyright Copyright (c) 2020
 * 
 * @todo Switch Password to .ini file \n
 * https://community.platformio.org/t/how-to-specify-ota-upload-password-outside-platformio-ini/3588 \n
 * http://docs.platformio.org/en/latest/envvars.html#building
 */

#ifndef COMMUNICATIONCONFIGURATION_H
#define COMMUNICATIONCONFIGURATION_H

//=============WLAN CONNECTION==================================
#define DEFAULT_WIFI_SSID "WG61"        ///< SSID to connect to
#define DEFAULT_WIFI_PASSWORD "C0meinandfind0ut"  ///< Password to corresponding SSID

// Pins for Adafruit ATWINC1500 Feather https://learn.adafruit.com/adafruit-feather-m0-wifi-atwinc1500/using-the-wifi-module
#define DEFAULT_WIFI_CS 8   ///< Chipselct pin
#define DEFAULT_WIFI_IRQ 7  ///< Interrupt  pin
#define DEFAULT_WIFI_RST 4  ///< Reset pin
#define DEFAULT_WIFI_EN 2   ///< Enable pin

//=============MQTT==================================
// * DEFAULT_MQTT_* are MQTT defaults
#define DEFAULT_MQTT_BROKER_IP1 192  ///< broker IP distributed in 4 values
#define DEFAULT_MQTT_BROKER_IP2 168  ///< broker IP distributed in 4 values
#define DEFAULT_MQTT_BROKER_IP3 0    ///< broker IP distributed in 4 values
#define DEFAULT_MQTT_BROKER_IP4 11   ///< broker IP distributed in 4 values
#define DEFAULT_MQTT_PORT 1883       ///< MQTT connection port

//=============JSON==================================
#define MAX_JSON_PARSE_SIZE 300     ///< max buffer size to parse JSON objects, size of the pool in bytes, can be calculated in https://arduinojson.org/v5/assistant/
#define MAX_JSON_MESSAGES_SAVED 20  ///< max num of saved JSON items, must be smaller than num of vehicles!

#endif