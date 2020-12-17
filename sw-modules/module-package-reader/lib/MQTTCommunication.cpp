/**
 * @file MQTTCommunication.cpp
 * @brief Implementation of the Connection class
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added Connection-Implementation - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-16
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */

/*
Useful Links
https://techtutorialsx.com/2017/04/29/esp32-sending-json-messages-over-mqtt/
https://randomnerdtutorials.com/decoding-and-encoding-json-with-arduino-or-esp8266/
https://arduinodiy.wordpress.com/2017/07/30/json-mqtt-and-openhab/
https://assetwolf.com/learn/sending-data-from-arduino-to-cloud
*/

#include "MQTTCommunication.h"

//==Global Vairable====
myJSON _myjson(MAX_JSON_PARSE_SIZE);                         ///< instance of myJSON
CircularBuffer<myJSONStr, MAX_JSON_MESSAGES_SAVED> _buffer;  ///< instance of CircularBuffer

//======Func====
/**
 * @todo Change global implementation of callback. Maybe with static?
 */
void callback(char* topic, byte* payload, unsigned int length) {
    DBFUNCCALLln("callback(const char[] topic, byte* payload, unsigned int length)");
    char payload_str[length];
    for (unsigned int i = 0; i < length; i++) {  // iterate message till lentgh caus it's not 0-terminated
        payload_str[i] = (char)payload[i];
    }
    payload_str[length] = '\0';

    String topic_str = String((char*)topic);
    String currentMessage = topic_str + " " + payload_str;
    DBINFO3("CurrMessage: ");
    DBINFO3ln(currentMessage);
    DBINFO3("LastMessage: ");
    DBINFO3ln(_myjson.lastMessage);

    if ((_myjson.lastMessage == currentMessage) && (_buffer.size() != 0)) {
        DBINFO2ln("Duplicated Message");
    } else {
        DBINFO2ln("Add to Buffer");
        //https://stackoverflow.com/questions/1360183/how-do-i-call-a-non-static-method-from-a-static-method-in-c
        myJSONStr newMessage = _myjson.parsingJSONToStruct((char*)payload_str);
        newMessage.topic = topic_str;
        DBINFO3("ID: ");
        DBINFO3ln(newMessage.id);
        DBINFO3("Topic: ");
        DBINFO3ln(newMessage.topic);
        DBINFO3("Status: ");
        DBINFO3ln(newMessage.status);
        DBINFO3("sector: ");
        DBINFO3ln(newMessage.sector);
        DBINFO3("line: ");
        DBINFO3ln(newMessage.line);
        DBINFO3("ack: ");
        DBINFO3ln(newMessage.ack);
        DBINFO3("req: ");
        DBINFO3ln(newMessage.req);
        DBINFO3("cargo: ");
        DBINFO3ln(newMessage.cargo);
        DBINFO3("error: ");
        DBINFO3ln(newMessage.error);
        DBINFO3("token: ");
        DBINFO3ln(newMessage.token);
        // _myjson.StructIsEqual(newMessage, _buffer.first()); //bug this calls changes the message chars somehow?
        _buffer.unshift(newMessage);
        _myjson.lastMessage = currentMessage;
    }
}

Communication::Communication(String Hostname) : pHostname(Hostname) {
    init();
}
