int rawSensorValue;
int filteredSensorValue;
const int numberOfFilters = 1; //Array Positionen von 0 bis numberOfFilters-1
const int filterLength = 7; //Ungerade Zahl wählen!

void setup()
{
  Serial.begin(9600);
  rawSensorValue = analogRead(A1);
  InitializeFilter(0,rawSensorValue);
}

void loop()
{
  rawSensorValue = analogRead(A1);
  UpdateFilter(0,rawSensorValue);
  filteredSensorValue = getFilterValue(0);
  Serial.print("Raw Value:  ");
  Serial.print(rawSensorValue);
  Serial.print("  Filtered Sensor Value:  ");
  Serial.println(filteredSensorValue);
  delay(100);
}
