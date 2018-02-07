#include <SendData2Slave.h>

bool SendData2Slave::doIt(uint8_t *command)
{
  this->bluetoothMaster.write("setMotors(0)\n");
  return true;
}
