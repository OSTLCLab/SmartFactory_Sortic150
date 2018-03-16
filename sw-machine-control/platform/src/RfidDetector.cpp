#include <Debug.h>
#include <RfidDetector.h>

byte *RfidDetector::loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return componentData;
  }

  debugLn("Card detected");
  byte blockSize = 18;
  MFRC522::StatusCode status{mfrc522.MIFARE_Read(0, componentData, &blockSize)};
  debugLn("Actual RFIDScanner status[" + String(status) + "] Status Ok[" + String(MFRC522::STATUS_OK) + "]");

  //If status is not ok, set componentstate invalid
  state = status == MFRC522::STATUS_OK ? Finish : Invalid;

  return componentData;
}
