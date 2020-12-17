/**
 * @file myMQTT.h
 * @brief This class handels the MQTT connection
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 - added MQTT-Connection - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 * 
 * @date 2019-04-16
 * @copyright Copyright (c) 2019
 * 
 * @todo Add a function "unsubscirbeAll" which unsubscribes from all subscirbed topics.
 * Hence you need to track all your subscribed topcis in an array.
 */

#ifndef MYMQTT_H__
#define MYMQTT_H__

#include "Arduino.h"

#include "LogConfiguration.h"

#include "Network/Network.h"
// #include <WiFi101.h>
#include <PubSubClient.h>

/**
 * @brief Handels the MQTT connection
 * 
 * https://pubsubclient.knolleary.net
 */
class myMQTT {
    //=====PUBLIC====================================================================================
   public:
    /**
     * @brief Construct a new my M Q T T object
     * 
     * @param hostname - the client ID to use when connecting to the server
     * @param BrokerIP1 - broker IP distributed in 4 values (192)
     * @param BrokerIP2 - broker IP distributed in 4 values (186)
     * @param BrokerIP3 - broker IP distributed in 4 values (1)
     * @param BrokerIP4 - broker IP distributed in 4 values (2)
     * @param BrokerPort - MQTT connection port
     */
    myMQTT(String hostname, const int BrokerIP1, const int BrokerIP2, const int BrokerIP3, const int BrokerIP4, const int BrokerPort);

    /**
     * @brief 
     * 
     * @param myClient - pointer to WiFiClient object
     * @param pToCallback - pointer to Callback-Function
     */
    void init(WiFiClient *myClient, void (*pToCallback)(char *, unsigned char *, unsigned int));

    /**
     * @brief subsribes to MQTT topic on Server
     * 
     *  Check if the client is connected to the server
     *  if not call \link connectToMQTT() \endlink
     * 
     * @param topic - the topic to subscribe to (const char[])
     * @return true -  sending the subscribe succeeded. The request completes asynchronously.
     * @return false - sending the subscribe failed, either connection lost, or message too large.
     */
    bool subscribe(const String topic);

    /**
     * @brief unsubsribes from MQTT topic on Server
     * 
     *  Check if the client is connected to the server
     * 
     * @param topic - the topic to unsubscribe from (const char[])
     * @return true - sending the unsubscribe succeeded. The request completes asynchronously.
     * @return false - sending the unsubscribe failed, either connection lost, or message too large.
     */
    bool unsubscribe(const String topic);

    /**
     * @brief handles outgoing MQTT messages to Server
     * 
     * Beware though that the default packet size
     * supported by the PubSub client is 128 bytes.
     * You can increase this limit by editing the value
     * lof MQTT_MAX_PACKET_SIZE in PubSubClient.h
     * 
     * @param topic - the topic to publish to (const char[])
     * @param msg - the message to publish (const char[])
     * @return true - publish succed
     * @return false - publish failes
     */
    bool publishMessage(const String topic, const String msg);

    /**
     * @brief This should be called regularly
     * to allow the client to process incoming messages
     * and maintain its connection to the server.
     * 
     * @return true - he client is still connected
     * @return false - the client is no longer connected
     */
    bool loop() {
        if (!myMQTTclient.loop()) {
            connectToMQTT();
        } else {
            return true;
        }
        return myMQTTclient.loop();
    }

    //=====PRIVATE====================================================================================
   private:
    /**
     * @brief Connects to the MQTT-server
     * 
     * If the server isn't already connected  a new connection will be established
     * If the connection fails then there will be a 3 Sec dealy.
     */
    void connectToMQTT();

    /**
     * @brief Decodes the Error Values from MQTT state() and returns a description
     * 
     * @param errorcode - errorvalue from client
     * @return String - Errordescription
     */
    String decodeMQTTstate(int errorcode);

    /**
     * @brief Writes Error-Message about MQTT connection
     * 
     */
    void MQTTConnectionFailed();

    String pHostname;           ///< Contains Hostname/Client ID
    IPAddress pBrokerIP;        ///< the IP-address of the server/broker
    const int pBrokerPort;      ///< port to connect to MQTT
    PubSubClient myMQTTclient;  ///< instance of PubSubClient
};
#endif