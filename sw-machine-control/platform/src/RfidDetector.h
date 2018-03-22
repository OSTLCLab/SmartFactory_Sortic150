#ifndef RfidDetector_h
#define RfidDetector_h

#include <Component.h>
#include <MFRC522.h>

class RfidDetector : public Component<byte *>
{
public:
  RfidDetector(MFRC522 *mfrc522) : mfrc522{mfrc522} {}

protected:
  byte *loop()
  {
    if (!mfrc522->PICC_IsNewCardPresent())
    {
      return componentData;
    }

    byte blockSize = 18;
    MFRC522::StatusCode status{mfrc522->MIFARE_Read(0, componentData, &blockSize)};

    //If status is not ok, set componentstate invalid
    state = status == MFRC522::STATUS_OK ? Finish : Invalid;

    return componentData;
  }

private:
  MFRC522 *mfrc522;
};

#endif