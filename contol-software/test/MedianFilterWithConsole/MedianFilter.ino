//Median Filter V2.1, 8.4.2016
//Erstellt durch Severin Scherrer im Rahmen der Bachelorarbeit "Pick and Place Maschine" an der HSR Hochschule für Technik Rapperswil, Schweiz


//Diese Werte im Hauptprogram deklarieren!
/*
const int filterLength = 7; //Ungerade Zahl wählen!
const int numberOfFilters = 2; //Array Positionen von 0 bis numberOfFilters-1
*/

float Values[numberOfFilters][filterLength];
int Rank[numberOfFilters][filterLength];
int currentPositionInFilter[numberOfFilters];

void InitializeFilter(int filterNumber, int defaultValue)
{
  for(int i = 0;i<filterLength;i++)
  {
    Values[filterNumber][i] = defaultValue;
    Rank[filterNumber][i] = i;
    currentPositionInFilter[filterNumber] = 0;
  }
}

void UpdateFilter(int filterNumber, int newValueAsInt)
{
  float newValue = newValueAsInt + currentPositionInFilter[filterNumber]/10000.0;
  float oldValue = Values[filterNumber][currentPositionInFilter[filterNumber]];
  Values[filterNumber][currentPositionInFilter[filterNumber]] = newValue;
  
  //Rank aktuallisieren
  if(newValue == oldValue)
  {
    //Serial.println("==");
    //Do Nothing
  }
  else if(newValue>oldValue)
  {
    //Serial.println("Der neue Value ist groesser");
    
    int numbersDecreased = 0;
    for(int i = 0;i<filterLength;i++)
    {
      if((Values[filterNumber][i]<newValue)&&(Values[filterNumber][i]>=oldValue))
      {
        numbersDecreased +=1;
        Rank[filterNumber][i] -= 1;
      }
    }
    Rank[filterNumber][currentPositionInFilter[filterNumber]] += numbersDecreased;
  }
  
  else if((newValue<oldValue)&&(Rank[filterNumber][currentPositionInFilter[filterNumber]]!=0))
  {
    //Serial.println("Der neue Wert ist kleiner");
    int numbersDecreased = 0;
    for(int i = 0;i<filterLength;i++)
    {
      if((Values[filterNumber][i]>newValue)&&(Values[filterNumber][i]<oldValue))
      {
        numbersDecreased +=1;
        Rank[filterNumber][i] += 1;
      }
    }
    Rank[filterNumber][currentPositionInFilter[filterNumber]] -= numbersDecreased;
    if(numbersDecreased == 0)
    {
      
    }
  }
  //Position aktualllisieren
  int returnPosition = currentPositionInFilter[filterNumber];
  if(currentPositionInFilter[filterNumber] == filterLength-1)
  {
    currentPositionInFilter[filterNumber] = 0;
  }
  else
  {
    currentPositionInFilter[filterNumber] += 1;
  }
}

int getFilterValue(int filterNumber)
{
  for(int i = 0; i<filterLength; i++)
  if(Rank[filterNumber][i]==filterLength/2)
  {
    int returnValue = Values[filterNumber][i];
    return returnValue;
  }
}

void PlotFilter(int filterNumber)
{
  for(int i = 0;i<filterLength-1;i++)
  {
    int value = Values[filterNumber][i];
    Serial.print(value);
    Serial.print(" ");
  }
  int value = Values[filterNumber][filterLength-1];
  Serial.println(value);

  for(int i = 0;i<filterLength-1;i++)
  {
    Serial.print(Rank[filterNumber][i]);
    Serial.print(" ");
  }
  Serial.println((Rank[filterNumber][filterLength-1]));
  Serial.print("Aktuelle Position: ");
  Serial.println(currentPositionInFilter[filterNumber]);
}
