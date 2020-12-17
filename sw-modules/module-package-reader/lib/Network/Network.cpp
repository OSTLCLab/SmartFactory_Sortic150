/**
 * @file Network.cpp
 * @brief Implementation of the Network class
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added Network-Connection - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-13
 * 
 * @date 2019-04-13
 * @copyright Copyright (c) 2019
 * 
 */

#include "Network.h"
//=====PUBLIC====================================================================================

Network::Network(String ssid, String password, int WIFI_CS, int WIFI_IRQ, int WIFI_RST, int WIFI_EN) : pSsid(ssid),
                                                                                                       pPassword(password),
                                                                                                       pWifi_CS(WIFI_CS),
                                                                                                       pWiFi_IRQ(WIFI_IRQ),
                                                                                                       pWifi_RST(WIFI_RST),
                                                                                                       pWifi_EN(WIFI_EN) {
}

void Network::init() {
    DBFUNCCALLln("Network::init()");
    WiFi.setPins(pWifi_CS, pWiFi_IRQ, pWifi_RST, pWifi_EN);
    if (WiFi.status() == WL_NO_SHIELD) {  // check if the shield is presence
        DBERROR("NO WiFi shield present");
        DBERROR("WiFi Library could not find WiFi shield. " + decodeWiFistate(WiFi.status()));
        DBINFO3ln("programm is not continuing");
        while (true) {
            // don't continue
        }
    }
    DBINFO3ln(String("WiFi Firmware Version = ") + String(WiFi.firmwareVersion()));
    connectToWiFi();
}

void Network::connectToWiFi() {
    DBFUNCCALLln("Network::connectToWiFi()");
    while (WiFi.status() != WL_CONNECTED) {  // connect to Wifi network
        DBSTATUS("Status:  " + decodeWiFistate(WiFi.status()));
        DBINFO3ln("Attempting WLAN connection (WEP)...");
        DBINFO3ln("SSID: " + pSsid);
        // DBINFO1ln("PW: " + pPassword);
        if (WiFi.begin(pSsid, pPassword) != WL_CONNECTED) {
            DBERROR("WLAN connection failed");
            DBINFO3ln("trying again in 3 seconds");
            delay(3000);
        } else {
            pIPLocal = WiFi.localIP();
            WiFi.macAddress(pMac);
            pSsid = WiFi.SSID();
            WiFi.macAddress(pMac);
            WiFi.BSSID(pMacRouter);
            pRssi = WiFi.RSSI();
            pEncryption = WiFi.encryptionType();
            // hostname(hostname.c_str());
        }
    }
}

void Network::printNetworkInfo() {
    DBFUNCCALLln("Network::printNetworkInfo()");
    printWiFiData();
    printCurrentNet();
}

//=====PRIVATE====================================================================================

void Network::printWiFiData() {
    DBFUNCCALLln("Network::printWiFiData()");
    // print your WiFi shield's IP address:
    pIPLocal = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(pIPLocal);
    Serial.println(pIPLocal);
    // print your MAC address:
    WiFi.macAddress(pMac);
    Serial.print("MAC address: ");
    printMacAddress(pMac);
}

void Network::printCurrentNet() {
    DBFUNCCALLln("Network::printCurrentNet()");
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    // print the MAC address of the router you're attached to:
    WiFi.BSSID(pMacRouter);
    Serial.print("BSSID: ");
    printMacAddress(pMacRouter);
    // print the received signal strength:
    pRssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(pRssi);
    // print the encryption type:
    pEncryption = WiFi.encryptionType();
    Serial.print("Encryption Type: ");
    Serial.println(decodeEncryptionType(pEncryption));
    Serial.println();
}

void Network::printMacAddress(byte mac[]) {
    for (int i = 5; i >= 0; i--) {
        if (mac[i] < 16) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i > 0) {
            Serial.print(":");
        }
    }
    Serial.println();
}

String Network::decodeWiFistate(int errorcode) {
    DBFUNCCALLln("Network::decodeWiFistate(int errorcode)");
    switch (errorcode) {
        case WL_NO_SHIELD:
            return "WL_NO_SHIELD";
        case WL_IDLE_STATUS:
            return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL:
            return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED:
            return "WL_SCAN_COMPLETED";
        case WL_CONNECTED:
            return "WL_CONNECTED";
        case WL_CONNECT_FAILED:
            return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
            return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED:
            return "WL_DISCONNECTED";
        case WL_AP_LISTENING:
            return "WL_AP_LISTENING";
        case WL_AP_CONNECTED:
            return "WL_AP_CONNECTED";
        case WL_AP_FAILED:
            return "WL_AP_FAILED";
        case WL_PROVISIONING:
            return "WL_PROVISIONING";
        case WL_PROVISIONING_FAILED:
            return "WL_PROVISIONING_FAILED";
        default:
            return "Error";
    }
}

String Network::decodeEncryptionType(int errorcode) {
    DBFUNCCALLln("Network::decodeEncryptionType(int errorcode)");
    switch (errorcode) {
        case 2:
            return "TKIP (WPA)";
        case 5:
            return "WEP";
        case 4:
            return "CCMP (WPA)";
        case 7:
            return "NONE";
        case 8:
            return "AUTO ";
        default:
            return "Error";
    }
}