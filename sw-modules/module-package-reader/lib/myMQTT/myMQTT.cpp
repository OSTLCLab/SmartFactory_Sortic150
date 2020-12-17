/**
 * @file myMQTT.cpp
 * @brief Implementation of the myMQTT class
 * 
 * https://pubsubclient.knolleary.net
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added MQTT-Connection - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 * 
 * @date 2019-04-16
 * @copyright Copyright (c) 2019
 * 
 */

#include "myMQTT.h"

//=====PUBLIC====================================================================================
myMQTT::myMQTT(String hostname,
               const int BrokerIP1,
               const int BrokerIP2,
               const int BrokerIP3,
               const int BrokerIP4,
               const int BrokerPort) : pHostname(hostname),
                                       pBrokerIP(BrokerIP1,
                                                 BrokerIP2,
                                                 BrokerIP3,
                                                 BrokerIP4),
                                       pBrokerPort(BrokerPort) {
    // myMQTTclient.PubSubClient(pBrokerIP, pBrokerPort, pToCallback, *myClient);
}

void myMQTT::init(WiFiClient *myClient, void (*pToCallback)(char *, unsigned char *, unsigned int)) {
    DBFUNCCALLln("myMQTT::init()");
    myMQTTclient.setClient(*myClient);
    myMQTTclient.setServer(pBrokerIP, pBrokerPort);
    myMQTTclient.setCallback(pToCallback);
    connectToMQTT();  // connecting to MQTT-Broker
}

bool myMQTT::subscribe(const String topic) {  // subscribes to a new MQTT topic
    DBFUNCCALLln("myMQTT::subscribe(const String topic) ");
    connectToMQTT();
    if (myMQTTclient.connected()) {
        DBINFO3("Subscribing to: " + topic);
        DBINFO3ln(" Client ID: " + pHostname);
        if (myMQTTclient.subscribe(topic.c_str())) {
            DBINFO1ln("suscription succesful");
            return true;
        } else {
            DBINFO1ln("suscription failed");
            return false;
        }
    }
    return false;
}

bool myMQTT::unsubscribe(const String topic) {
    DBFUNCCALLln("myMQTT::unsubscribe(const String topic)");
    connectToMQTT();
    if (myMQTTclient.connected()) {
        DBINFO3("Unsubscribe from " + topic);
        DBINFO3ln(" Client ID: " + pHostname);
        if (myMQTTclient.unsubscribe(topic.c_str())) {
            DBINFO3ln("unsubscribed successfully");
            return true;
        } else {
            DBWARNINGln("unsubscribe failed");
            return false;
        }
    }
    return false;
}

bool myMQTT::publishMessage(const String topic, const String msg) {  // publishes a message to the server
    DBFUNCCALLln("myMQTT::publishMessage(const String topic, const String msg)");
    // DBINFO3ln("try to publish to[" + topic + "] message: " + msg);
    connectToMQTT();
    if (myMQTTclient.connected()) {
        if (myMQTTclient.publish(topic.c_str(), msg.c_str())) {
            DBINFO3ln("Publish to topic [" + topic + "] message:" + msg);
            return true;
        } else {
            DBWARNINGln("Publish failed");
            return false;
        }
    }
    return false;
}

//=====PRIVATE====================================================================================

void myMQTT::connectToMQTT() {
    DBFUNCCALLln("myMQTT::connectToMQTT()");
    /*
    * loop() should be called regularly to allow
    * the client to process incoming messages and
    * maintain its connection to the server.
    * https://pubsubclient.knolleary.net/api.html#loop 
    */
    myMQTTclient.loop();
    while (!myMQTTclient.connected()) {
        DBSTATUSln("Status:  " + decodeMQTTstate(myMQTTclient.state()));
        DBINFO3ln("Attempting MQTT connection...");
        DBINFO3ln("MQTT Client ID: " + pHostname);
        if (myMQTTclient.connect(pHostname.c_str())) {
            DBINFO3ln("MQTT has successfully connected with hostname: " + pHostname);
        } else {
            MQTTConnectionFailed();
            DBINFO3ln("trying again in 3 seconds");
            delay(5);
        }
    }
}

void myMQTT::MQTTConnectionFailed() {
    DBFUNCCALLln("NetworkManager::MQTTConnectionFailed()");
    DBERROR("MQTT connection failed: " + decodeMQTTstate(myMQTTclient.state()));
    DBINFO3ln("client status: " + decodeMQTTstate(myMQTTclient.state()));
}

String myMQTT::decodeMQTTstate(int errorcode) {
    DBFUNCCALLln("myMQTT::decodeMQTTstate(int errorcode)");
    switch (errorcode) {
        case -4:
            return "MQTT_CONNECTION_TIMEOUT";
        case -3:
            return "MQTT_CONNECTION_LOST";
        case -2:
            return "MQTT_CONNECT_FAILED";
        case -1:
            return "MQTT_DISCONNECTED";
        case 0:
            return "MQTT_CONNECTED";
        case 1:
            return "MQTT_CONNECT_BAD_PROTOCOL";
        case 2:
            return "MQTT_CONNECT_BAD_CLIENT_ID";
        case 3:
            return "MQTT_CONNECT_UNAVAILABLE";
        case 4:
            return "MQTT_CONNECT_BAD_CREDENTIALS";
        case 5:
            return "MQTT_CONNECT_UNAUTHORIZED";
        default:
            return "Error";
    }
}
