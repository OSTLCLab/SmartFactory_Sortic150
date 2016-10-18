//Nach Serial Event Vorlage

String myString = "";
int defaultValue = 5;

const int numberOfFilters = 1; //Array Positionen von 0 bis numberOfFilters-1
const int filterLength = 5; //Ungerade Zahl wählen!


void setup() {
  // initialize serial:
  Serial.begin(9600);
  InitializeFilter(0,defaultValue);
  delay(2000);
  Serial.println("Setup complete");
}

void loop()
{
  while (Serial.available())
  {
    int ch = Serial.read();
    if (ch != '.') // \n = Zeilenumbruch
    {
      //Serial.println("rec");
      myString += char(ch);
    }
    else
    {
      int myNumber = myString.toInt();
      UpdateFilter(0,myNumber);
      int result = getFilterValue(0);
      Serial.print("the Median value is: ");
      Serial.println(result);
      Serial.println("The values in the filter are");
      PlotFilter(0);
      Serial.println("");
      // clear the string:
      myString = "";
    }
  }
}


