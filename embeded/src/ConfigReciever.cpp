#include <Arduino.h>
#include <ArduinoJson.h>
#include <ConfigReciever.h>
#include <Placer.h>

Config ConfigReciever::loop()
{
  if (!Serial.available())
  {
    state = State::Finish;
    return sensorData;
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
    return sensorData;
  }

  if (root.containsKey("powerOn"))
  {
    sensorData.powerOn = root["powerOn"];
    Serial.println("Power [" + String(sensorData.powerOn) + "]");
  }

  if (root.containsKey("startPosition"))
  {
    sensorData.startPosition = root["startPosition"];
    Serial.println("startPosition [" + String(sensorData.startPosition) + "]");
  }

  if (root.containsKey("sizeOfRfidChips"))
  {
    sensorData.sizeOfRfidChips = root["sizeOfRfidChips"];
    Serial.println("SizeOfRfidChips [" + String(sensorData.sizeOfRfidChips) + "]");
  }

  if (root.containsKey("index") && root.containsKey("position"))
  {
    int index = root["index"];
    sensorData.rfidChips[index].targetPosition = root["position"];
    Serial.println("RfidChip [" + String(index) + "] targetPosition [" + String(sensorData.rfidChips[index].targetPosition) + "]");
  }

  if (root.containsKey("index") && root.containsKey("direction"))
  {
    int index = root["index"];
    int directionAsInt = root["direction"];
    sensorData.rfidChips[index].placerPosition = (PlacerPosition)directionAsInt;
    Serial.println("RfidChip [" + String(index) + "] targetDirection [" + String(directionAsInt) + "]");
  }

  if (root.containsKey("index") && root.containsKey("nr"))
  {
    int index = root["index"];
    JsonArray &rfidNr = root["nr"];

    Serial.print("RfidChip [" + String(index) + "] nr [");
    for (int i = 0; i < RFID_LENGTH; i++)
    {
      sensorData.rfidChips[index].rfidChip[i] = (byte)rfidNr[i];
      Serial.print(sensorData.rfidChips[index].rfidChip[i]);
      i == RFID_LENGTH - 1 ? Serial.println("]") : Serial.print(",");
    };
  }

  Serial.flush();
  buffer.clear();

  state = State::Finish;

  return sensorData;
}
