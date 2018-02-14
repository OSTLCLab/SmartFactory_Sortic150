#ifndef RfidDetector_h
#define RfidDetector_h

#include <Component.h>
#include <MFRC522.h>

class RfidDetector : public Component<byte *>
{
public:
  RfidDetector(MFRC522 &mfrc522) : mfrc522{mfrc522} {}

protected:
  byte *loop();

private:
  MFRC522 &mfrc522;
};

#endif
