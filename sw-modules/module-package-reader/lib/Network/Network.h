/**
 * @file Network.h
 * @brief The Network class establishes a WLAN-Connection
 * 
 * https://www.arduino.cc/en/Reference/WiFi101
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added Network-Connection - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-13
 * 
 * @date 2019-04-13
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef NETWORK_H__
#define NETWORK_H__

#include "Arduino.h"

#include "LogConfiguration.h"

#include <SPI.h>
#include <WiFi101.h>

/**
 * @brief Handels the WLAN-Connection
 * 
 */
class Network {
    //=====PUBLIC====================================================================================
   public:
       /**
     * @brief Construct a new Network object
     * 
     * @param ssid -  SSID of the WLan you would like to connect
     * @param password - Password of the desired WLAN
     * @param WIFI_CS - 
     * @param WIFI_IRQ - 
     * @param WIFI_RST - 
     * @param WIFI_EN - 
     */
    Network(String ssid, String password, int WIFI_CS, int WIFI_IRQ, int WIFI_RST, int WIFI_EN);

    /**
     * @brief Intitialize WLan-Hardware
     * 
     * Sets Pins from Communication-Config file and
     * checks if a WiFi-Shield is present.
     * If not the programm stops
     * else it calls connectToWiFi()
     * 
     */
    void init();

    /**
     * @brief Connects to a WiFi with the given Credential
     * 
     * If the Shield isn't already connected a new connection will be established
     * If the Connection fails then there will be a 3 Sec dealy.
     * https://www.arduino.cc/en/Tutorial/Wifi101ConnectWithWPA 
     */
    void connectToWiFi();

    /**
     * @brief Prints all relevant Network-Information
     * of the connected network to serial
     * 
     * IP-Address, MAC address (printWiFiData)
     * SSID, BSSID, RSSI and encryptionType (printCurrentNet)
     * 
     */
    void printNetworkInfo();

    //=====PRIVATE====================================================================================
   private:
    /**
     * @brief Prints all relevant WiFi-Information
     * of the connected network to serial
     * 
     * IP-Address, MAC address
     */
    void printWiFiData();

    /**
     * @brief Prints all relevant Information of the current Network
     * 
     *  SSID, BSSID, RSSI and encryptionType
     */
    void printCurrentNet();

    /**
     * @brief Pritns Mac-address
     * 
     * @param mac - 
     */
    void printMacAddress(byte mac[]);

    /**
     * @brief Decodes the Error Values from Wifi status() and returns a description
     * 
     * @param errorcode - errorvalue from wifi101
     * @return String - Errordescription
     */
    String decodeWiFistate(int errorcode);

    /**
     * @brief Construct a new decode Encryption Type object
     * 
     * https://www.arduino.cc/en/Reference/WiFi101EncryptionType
     * 
     * 2 = TKIP (WPA) \n
     * 5 = WEP  \n
     * 4 = CCMP (WPA)\n
     * 7 = NONE \n
     * 8 = AUTO \n
     * 
     * @param errorcode - errorvalue from wifi101
     * @return String - Errordescription
     */
    String decodeEncryptionType(int errorcode);

    String pSsid;        ///< Contains the SSID the WiFi shield is currently connected to.
    String pPassword;    ///< Contains WiFi Password
    int pWifi_CS;
     int      pWiFi_IRQ;
      int                                                                                                 pWifi_RST;
         int                                                                                              pWifi_EN;
    IPAddress pIPLocal;  ///< Contains own IP-Adress
    byte pMacRouter[6];  ///< Contains MAC Adress of the Router BSSID
    byte pMac[6];        ///< Contains own MAC Adress
    byte pEncryption;    ///< value represents the type of encryption

    /**
     * @brief The current RSSI /Received Signal Strength in dBm
     * 
     * -30 dBm	Maximum signal strength, you are probably standing right next to the access point.	\n
     * -50 dBm	Anything down to this level can be considered excellent signal strength.	\n
     * -60 dBm	Good, reliable signal strength.	\n
     * -67 dBm	Reliable signal strength. The minimum for any service depending on a reliable connection and signal strength, such as voice over Wi-Fi and non-HD video streaming.\n
     * -70 dBm	Not a strong signal. Light browsing and email.\n
     * -80 dBm	Unreliable signal strength, will not suffice for most services.	Connecting to the network.\n
     * -90 dBm	The chances of even connecting are very low at this level.\n
     * Source https://eyesaas.com/wi-fi-signal-strength/
     */
    long pRssi;
};
#endif
