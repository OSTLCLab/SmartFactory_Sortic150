void setup()
{
  Serial.begin(9600);
  //while(!Serial);
  myTest();
}

void loop()
{
  myTest();
  delay(1000);
}
