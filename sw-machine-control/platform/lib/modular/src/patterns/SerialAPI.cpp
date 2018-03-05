#include <patterns/SerialAPI.h>

SerialAPI::SerialAPI(HardwareSerial *iSerialCom) {
  serialCom = iSerialCom;
  apiBuffer = "";
};

void SerialAPI::handleSerialInstruction(String task, String value) {
   // Implemnation need to be done in the derived class for now
   serialCom->println("Task revieced: " + task + " : " + value);
}

void SerialAPI::loop() {
  while (serialCom->available() > 0) {


    if (serialCom->available() > 0) {
         char c = serialCom->read();
         apiBuffer += c;

        if (c == ';') {
          String order = apiBuffer.substring(0, apiBuffer.length() - 1);
          order.trim();
          int j = order.indexOf(':');
          String task = order.substring(0, j);
          String value = order.substring(j+1);
          task.trim();
          value.trim();
          this->handleSerialInstruction(task, value);
          apiBuffer = "";
        }
    }
  }
}

void SerialAPI::sendJSON(String key, String value) {
  serialCom->println("{" + key + "; " + value + "}");
}
