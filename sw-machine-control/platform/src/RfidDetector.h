#ifndef RfidDetector_h
#define RfidDetector_h

#include <Component.h>
#include <MFRC522.h>
#include <MachineAPI.h>

class RfidDetector : public Component<int>
{
public:
  RfidDetector(MFRC522 *mfrc522, Component<Config> *machineApi) : mfrc522{mfrc522}, machineApi{machineApi}
  {
    componentData = -1;
  }

protected:
  int loop()
  {
    if (!mfrc522->PICC_IsNewCardPresent())
    {
      return -1;
    }
    debugLn("PICC_IsNewCardPresent");
    byte buffer[18];
    byte size = sizeof(buffer);
    MFRC522::StatusCode status = mfrc522->MIFARE_Read(0, buffer, &size);

    //If status is not ok, set componentstate invalid
    state = status == MFRC522::STATUS_OK ? State::Finish : State::Invalid;
    debugLn(String(state) + " " + String(status) + " " + String(size));

    return getIndexOfRFidChip(buffer);
  }

private:
  MFRC522 *mfrc522;
  Component<Config> *machineApi;

  int getIndexOfRFidChip(byte *id)
  {
    for (int index = 0; index < RFID_LENGTH; index++)
    {
      if (!memcmp(id, machineApi->getData().rfids[index].id, RFID_LENGTH * sizeof(byte)))
      {
        debugLn();
        return index;
      }
    }

    return RFID_LENGTH - 1;
  }
};

#endif