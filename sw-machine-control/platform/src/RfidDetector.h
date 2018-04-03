#ifndef RfidDetector_h
#define RfidDetector_h

#include <Component.h>
#include <MFRC522.h>

class RfidDetector : public Component<byte *>
{
public:
  RfidDetector(MFRC522 *mfrc522) : mfrc522{mfrc522}
  {
  }

protected:
  byte *loop()
  {
    if (!mfrc522->PICC_IsNewCardPresent())
    {
      return componentData;
    }
    debugLn("PICC_IsNewCardPresent");
    byte buffer[18];
    byte size = sizeof(buffer);
    MFRC522::StatusCode status = mfrc522->MIFARE_Read(0, buffer, &size);

    //If status is not ok, set componentstate invalid
    state = status == MFRC522::STATUS_OK ? State::Finish : State::Invalid;
    debugLn(String(state) + " " + String(status) + " " + String(size));
    componentData = buffer;

    return componentData;
  }

private:
  MFRC522 *mfrc522;
};

#endif