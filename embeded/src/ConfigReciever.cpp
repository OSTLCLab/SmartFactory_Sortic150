#include <Arduino.h>
#include <ArduinoJson.h>
#include <ConfigReciever.h>
#include <PlacerPerformance.h>

Config ConfigReciever::loop()
{
  if (!Serial.available())
  {
    state = State::Finish;
    return data;
  }

  Serial.println("Recieve Message.");
  String readedString = Serial.readString();

  JsonObject &root = buffer.parseObject(readedString);

  if (!root.success())
  {
    Serial.println("Parsing JSON fail. Bad format.");
    Serial.println(readedString);
    buffer.clear();
    state = State::Invalid;
    return data;
  }

  if (root.containsKey("powerOn"))
  {
    data.powerOn = root["powerOn"];
    Serial.println("Power [" + String(data.powerOn) + "]");
  }

  if (root.containsKey("startPosition"))
  {
    data.startPosition = root["startPosition"];
    Serial.println("startPosition [" + String(data.startPosition) + "]");
  }

  if (root.containsKey("sizeOfRfidChips"))
  {
    data.sizeOfRfidChips = root["sizeOfRfidChips"];
    Serial.println("SizeOfRfidChips [" + String(data.sizeOfRfidChips) + "]");
  }

  if (root.containsKey("index") && root.containsKey("position"))
  {
    int index = root["index"];
    data.rfidChips[index].targetPosition = root["position"];
    Serial.println("RfidChip [" + String(index) + "] targetPosition [" + String(data.rfidChips[index].targetPosition) + "]");
  }

  if (root.containsKey("index") && root.containsKey("direction"))
  {
    int index = root["index"];
    int directionAsInt = root["direction"];
    data.rfidChips[index].targetDirection = (PlacerActionDirection)directionAsInt;
    Serial.println("RfidChip [" + String(index) + "] targetDirection [" + String(directionAsInt) + "]");
  }

  if (root.containsKey("index") && root.containsKey("nr"))
  {
    int index = root["index"];
    JsonArray &rfidNr = root["nr"];

    Serial.print("RfidChip [" + String(index) + "] nr [");
    for (int i = 0; i < RFID_LENGTH; i++)
    {
      data.rfidChips[index].rfidChip[i] = (byte)rfidNr[i];
      Serial.print(data.rfidChips[index].rfidChip[i]);
      i == RFID_LENGTH - 1 ? Serial.println("]") : Serial.print(",");
    };
  }

  Serial.flush();
  buffer.clear();

  state = State::Finish;

  return data;
}
