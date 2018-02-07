#include <SoftwareSerial.h>
#include <Command.h>

#ifndef senddata2slave_h
#define senddata2slave_h

class SendData2Slave : public Command
{
public:
  SendData2Slave(SoftwareSerial &bluetoothMaster) : bluetoothMaster{bluetoothMaster} {}
  bool doIt(uint8_t *command);

private:
  SoftwareSerial &bluetoothMaster;
};

#endif
