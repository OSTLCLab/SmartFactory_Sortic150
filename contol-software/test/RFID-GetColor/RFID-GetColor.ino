/*
This program shows what part is identified by the RFID Reader
New RFID ID's are automaticly read out if not recognised
The program is optimized to use of MIFARE Ultralight stickers
*/

#include <SPI.h>
#include <MFRC522.h>

byte teilSchwarz[8] = {4, 135, 115, 120, 162, 231, 73, 128};
byte teilGelb[8] = {4, 28, 116, 228, 162, 231, 73, 128};
byte teilGrauGelb[8] = {4, 161, 115, 94, 162, 231, 73, 128};
byte teilGrauSchwarz[8] = {4, 42, 117, 211, 162, 231, 73, 128};

MFRC522 RfidPart(6,5);
MFRC522 RfidBox(10,9);

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  
  SPI.begin();
  RfidPart.PCD_Init();
  //RfidBox.PCD_Init();
  
  Serial.println("Init complete");
}

void loop()
{
  if (RfidPart.PICC_IsNewCardPresent())
  {
    //Serial.println("Card found");
    
    byte blockAddr = 0;
    byte bufferSize = 18;
    byte bufferArray[bufferSize];
    byte outputSize = 8;
    
    MFRC522::StatusCode status;

    status = (MFRC522::StatusCode) RfidPart.MIFARE_Read(blockAddr, bufferArray, &bufferSize);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: ")); Serial.println(RfidPart.GetStatusCodeName(status));
    }
    
    if(ArrayIsEqual(bufferArray, teilSchwarz))
    {
      Serial.println("Das Teil ist schwarz");
    }
    else if(ArrayIsEqual(bufferArray, teilGelb))
    {
      Serial.println("Das Teil ist gelb");
    }
    else if(ArrayIsEqual(bufferArray, teilGrauGelb))
    {
      Serial.println("Das Teil ist grau-gelb");
    }
    else if(ArrayIsEqual(bufferArray, teilGrauSchwarz))
    {
      Serial.println("Das Teil ist grau-schwarz");
    }
    else
    {
      Serial.println("Das Teil ist nicht bekannt");
      Serial.print("Die ID ist: ");
      for (byte i = 0; i < outputSize; i++)
      {
        //Serial.print(bufferArray[i] < 0x10 ? " 0" : " ");
        Serial.print(bufferArray[i]);
        Serial.print(", ");
        if(i%8 == 7)
        {
          Serial.println("");
        }
      }
    }
    
    Serial.println();
    delay(200);
  }
}
