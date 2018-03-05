#include <Arduino.h>

#ifndef SerialAPI_h
#define SerialAPI_h

class SerialAPI
{
  public:
    SerialAPI(HardwareSerial *iSerialCom);
    void loop();
    virtual void handleSerialInstruction(String task, String value);
    void sendJSON(String key, String value);
  protected:
    String apiBuffer;
    HardwareSerial *serialCom;
};

#endif
