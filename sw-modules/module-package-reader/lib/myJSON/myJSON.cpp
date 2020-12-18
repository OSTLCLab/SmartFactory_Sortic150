/**
 * @file myJSON.cpp
 * @brief Implementation of the myJSON-class
 *
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 *
 * @version 1.0 - added JSON-Conversion - Luca Mazzoleni (luca.mazzoleni@hsr.ch) - 2019-04-17
 *
 * @date 2019-04-09
 * @copyright Copyright (c) 2019
 *
 */

#include "myJSON.h"

myJSON::myJSON(const size_t ParsCapacity) : pParsCapacity(ParsCapacity) {
}

myJSONStr myJSON::parsingJSONToStruct(const char* json) {
    DBFUNCCALLln("myJSON::parsingJSONToStruct(const char* json)");
    DBINFO3ln(json);
    myJSONStr tempStr;
    DynamicJsonDocument doc(pParsCapacity);

    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        DBWARNING("deserializeJson() failed: ");
        DBWARNINGln(error.c_str());
        tempStr.id = "Pars failed";
        return tempStr;
    } else {
        // DBINFO1("deserializeJson() success: ");
        /* use excplicit cast to prevent ambiguity
         * https://arduinojson.org/v6/doc/deserialization/ S67
         */

        tempStr.id = doc["id"].as<String>();          // "SV46465"
        tempStr.status = doc["status"].as<String>();  // "driving"
        tempStr.sector = doc["sector"].as<String>();  // "sector"
        tempStr.line = doc["line"].as<int>();         // 1
        tempStr.ack = doc["ack"].as<String>();        // "hostname"
        tempStr.req = doc["req"].as<String>();        // "hostname"
        tempStr.cargo = doc["cargo"].as<String>();    // "gemuse"
        tempStr.token = doc["token"].as<bool>();      // false
        tempStr.error = doc["error"].as<bool>();      // false
        return tempStr;
    }
}

//Serializing program ===================================================
// String myJSON::serializ(myJSONStr tempStr) {
//     // DynamicJsonDocument doc(pSerializCapacity);

//     // doc["urgent"] = tempStr.urgent;
//     // doc["topic"] = tempStr.topic;
//     // doc["hostname"] = tempStr.hostname;
//     // doc["request"] = tempStr.request;
//     // doc["level"] = tempStr.level;

//     // JsonArray vehicleParams = doc.createNestedArray("vehicleParams");
//     // vehicleParams.add(tempStr.vehicleParams);
//     // // vehicleParams.add(tempStr.vehicleParams[0]);
//     // // vehicleParams.add(tempStr.vehicleParams[1]));
//     // // vehicleParams.add(tempStr.vehicleParams[2]));
//     // // vehicleParams.add(tempStr.vehicleParams[3]));
//     // // vehicleParams.add(tempStr.vehicleParams[4]));

//     // serializeJson(doc, Serial);
//     // // serializeJsonPretty(doc, Serial);
// }
