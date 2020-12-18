/**
 * @file LogConfiguration.h
 * @brief Contains Pre-Compiler directives for diffent Serialprints for Datalogin
 * 
 * 
 * @author Luca Mazzoleni (luca.mazzoleni@hsr.ch)
 * 
 * @version 1.0 -  Implement diffrent debug functions - Luca Mazzoleni (luca.mazzoleni@hsr.ch)  - 2019-03-20
 * 
 * @date 2019-03-20
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef LOGCONFIGURATION_H
#define LOGCONFIGURATION_H

#define DEBUGGER false  ///< Option to activate the Serial.print global

#ifdef DEBUGGER
#define DEBUG_ERROR     ///< Define DEBUG_ERROR global to print all  occuring errors via serial
#define DEBUG_WARNING   ///< Define DEBUG_WARNING global to print all occuring warnings via serial
#define DEBUG_STATUS    ///< Define DEBUG_STATUS global to print all occuring FSM status-updates via serial
#define DEBUG_EVENT     ///< Define DEBUG_EVENT global to print all occuring EVENTS which lead to a statechange via serial
#define DEBUG_INFO1     ///< Define DEBUG_INFO1 global to print all occuring FSM status changes (entering/leaving) via serial
#define DEBUG_INFO2     ///< Define DEBUG_INFO2 global to print all occuring infos inside the FSM via serial
#define DEBUG_INFO3     ///< Define DEBUG_INFO3 global to print all occuring message on hardwarelevel via serial
#define DEBUG_FUNCCALL  ///< Define DEBUG_FUNCCALL global to print all occuring functioncalls via serial
#endif

#ifdef DEBUG_ERROR
#define DBERROR(x)           \
    Serial.print("ERROR: "); \
    Serial.println(x);
#else
#define DBERROR(x)
#endif

#ifdef DEBUG_WARNING
#define DBWARNING(x) Serial.print(x)
#define DBWARNINGln(x) Serial.println(x)
#else
#define DBWARNING(x)
#define DBWARNINGln(x)
#endif

#ifdef DEBUG_STATUS
#define DBSTATUS(x) Serial.print(x)
#define DBSTATUSln(x) Serial.println(x)
#else
#define DBSTATUS(x)
#define DBSTATUSln(x)
#endif

#ifdef DEBUG_EVENT
#define DBEVENT(x) Serial.print(x)
#define DBEVENTln(x) Serial.println(x)
#else
#define DBEVENT(x)
#define DBEVENTln(x)
#endif

#ifdef DEBUG_INFO1
#define DBINFO1(x)       \
    if (Serial) {        \
        Serial.print(x); \
    };
#define DBINFO1ln(x)       \
    if (Serial) {          \
        Serial.println(x); \
    };
#else
#define DBINFO1(x)
#define DBINFO1ln(x)
#endif

#ifdef DEBUG_INFO2
#define DBINFO2(x)       \
    if (Serial) {        \
        Serial.print(x); \
    };
#define DBINFO2ln(x)       \
    if (Serial) {          \
        Serial.println(x); \
    };
#else
#define DBINFO2(x)
#define DBINFO2ln(x)
#endif

#ifdef DEBUG_INFO3
#define DBINFO3(x)       \
    if (Serial) {        \
        Serial.print(x); \
    };
#define DBINFO3ln(x)       \
    if (Serial) {          \
        Serial.println(x); \
    };
#else
#define DBINFO3(x)
#define DBINFO3ln(x)
#endif

#ifdef DEBUG_FUNCCALL
#define DBFUNCCALL(x)    \
    if (Serial) {        \
        Serial.print(x); \
    }
#define DBFUNCCALLln(x)    \
    if (Serial) {          \
        Serial.println(x); \
    }
#else
#define DBFUNCCALL(x)
#define DBFUNCCALLln(x)
#endif

#endif