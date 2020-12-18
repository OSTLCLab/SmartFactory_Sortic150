/**
 * @file myJSONStr.h
 * @brief myJSON Struct defines the format of the messages
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added JSON-Conversion - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-17
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */
#ifndef MYJSONSTR_H
#define MYJSONSTR_H

// {
//   "id": "hostanme",
//   "topic": "tpois/topic",
//   "status": "driving",
//   "sector": "transit",
//   "line": 1,
//   "ack":"hostname",
//   "req":"hostname",
//   "cargo":"gemuse",
//   "token":false;
//   "error":false
// }

/**
 * @brief myJSON Struct defines the format of the messages
 * 
 */
struct myJSONStr {
    String id = "-1";      ///< Hostname
    String topic = "-1";   ///< MQTT topic
    String status = "-1";  ///< Status
    String sector = "-1";  ///< Sector
    int line = -1;         ///< Line
    String ack = "-1";     ///< Acknoledgement
    String req = "-1";     ///< Request
    String cargo = "-1";   ///< Cargo
    bool token = false;    ///< Gateway and Reset-token
    bool error = false;    ///< Error-token
};
#endif