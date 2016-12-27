#include "MedianFilter.h"

MedianFilter::MedianFilter(int defaultValue) { //(int tempFilterLength)
  /*
  filterLenght = tempFilterLength;
  values = new float[tempFilterLength];
  rank = new int[tempFilterLength];
  */
  for(int i = 0;i<filterLength;i++)
  {
    values[i] = defaultValue;
    rank[i] = i;
    currentPositionInFilter = 0;
  }
}

void MedianFilter::UpdateFilter(int newValueAsInt) {
  float newValue = newValueAsInt + currentPositionInFilter/10000.0;
  float oldValue = values[currentPositionInFilter];
  values[currentPositionInFilter] = newValue;
  
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
      if((values[i]<newValue)&&(values[i]>=oldValue))
      {
        numbersDecreased +=1;
        rank[i] -= 1;
      }
    }
    rank[currentPositionInFilter] += numbersDecreased;
  }
  
  else if((newValue<oldValue)&&(rank[currentPositionInFilter]!=0))
  {
    //Serial.println("Der neue Wert ist kleiner");
    int numbersDecreased = 0;
    for(int i = 0;i<filterLength;i++)
    {
      if((values[i]>newValue)&&(values[i]<oldValue))
      {
        numbersDecreased +=1;
        rank[i] += 1;
      }
    }
    rank[currentPositionInFilter] -= numbersDecreased;
    if(numbersDecreased == 0)
    {
      
    }
  }
  //Position aktualllisieren
  int returnPosition = currentPositionInFilter;
  if(currentPositionInFilter == filterLength-1)
  {
    currentPositionInFilter = 0;
  }
  else
  {
    currentPositionInFilter += 1;
  }
}

int MedianFilter::getFilterValue()
{
  for(int i = 0; i<filterLength; i++)
  if(rank[i]==filterLength/2)
  {
    int returnValue = values[i];
    return returnValue;
  }
}
