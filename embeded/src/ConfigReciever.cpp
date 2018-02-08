#include <Arduino.h>
#include <ArduinoJson.h>
#include <ConfigReciever.h>

Config ConfigReciever::loop()
{
  if (!Serial.available())
  {
    return config;
  }
  Serial.println("Recieve Message.");
  String readedString = Serial.readString();

  JsonObject &root = buffer.parseObject(readedString);

  if (!root.success())
  {
    Serial.println("Parsing JSON fail. Bad format.");
    Serial.println(readedString);
    buffer.clear();
    return config;
  }

  if (root.containsKey("powerOn"))
  {
    config.powerOn = root["powerOn"];
    Serial.println("Power [" + String(config.powerOn) + "]");
  }

  if (root.containsKey("startPosition"))
  {
    config.startPosition = root["startPosition"];
    Serial.println("Startposition [" + String(config.startPosition) + "]");
  }

  if (root.containsKey("sizeOfRfidChips"))
  {
    config.sizeOfRfidChips = root["sizeOfRfidChips"];
    Serial.println("SizeOfRfidChips [" + String(config.sizeOfRfidChips) + "]");
  }

  if (root.containsKey("index") && root.containsKey("position"))
  {
    int index = root["index"];
    config.rfidChips[index].targetPosition = root["position"];
    Serial.println("RfidChip [" + String(index) + "] targetPosition [" + String(config.rfidChips[index].targetPosition) + "]");
  }

  if (root.containsKey("index") && root.containsKey("direction"))
  {
    int index = root["index"];
    int directionAsint = root["direction"];
    config.rfidChips[index].targetDirection = (PlacerActionDirection)directionAsint;
    Serial.println("RfidChip [" + String(index) + "] targetDirection [" + String((int)config.rfidChips[index].targetDirection) + "]");
  }

  if (root.containsKey("index") && root.containsKey("nr"))
  {
    int index = root["index"];
    JsonArray &rfidNr = root["nr"];

    Serial.print("RfidChip [" + String(index) + "] nr [");
    for (int i = 0; i < RFID_LENGTH; i++)
    {
      config.rfidChips[index].rfidChip[i] = (byte)rfidNr[i];
      Serial.print(config.rfidChips[index].rfidChip[i]);
      i == RFID_LENGTH - 1 ? Serial.println("]") : Serial.print(",");
    };
  }

  Serial.flush();
  buffer.clear();
}
