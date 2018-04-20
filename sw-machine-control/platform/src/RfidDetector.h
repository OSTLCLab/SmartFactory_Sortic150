#ifndef RfidDetector_h
#define RfidDetector_h

#include <Component.h>
#include <MFRC522.h>
#include <MachineAPI.h>

class RfidDetector : public Component<SortJob>
{
public:
  RfidDetector(MFRC522 *rfidReader) : rfidReader{rfidReader}
  {
    componentData = DEFAULT_SORTJOB;
  }

protected:
  SortJob loop()
  {
    if (!rfidReader->PICC_IsNewCardPresent())
    {
      return DEFAULT_SORTJOB;
    }

    if (!rfidReader->PICC_ReadCardSerial())
    {
      debugLn("ReadCardSerial was not successful");
      return DEFAULT_SORTJOB;
    }

    state = rfidReader->uid.size != 0 ? State::Finish : State::Invalid;

    debugLn(String(state));

    SortJob newChip{rfidReader->uid.uidByte, -1, NoPosition};

    return newChip;
  }

private:
  MFRC522 *rfidReader;
  Component<Config> *machineApi;
};

#endif