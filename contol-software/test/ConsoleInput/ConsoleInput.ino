//Nach Serial Event Vorlage

String myString = "";


void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  delay(2000);
  //while(!Serial.available());
  Serial.println("Setup complete");
}

void loop()
{
  while (Serial.available())
  {
    int ch = Serial.read();
    if (ch != '.') // \n = Zeilenumbruch
    {
      myString += char(ch);
    }
    else
    {
      Serial.println(myString);
      myString = "";
    }
  }
}


