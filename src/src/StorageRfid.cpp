#include <Arduino.h>
#include <SendData2Slave.h>
#include <StorageRfid.h>

bool StorageRfid::doIt(uint8_t *position)
{

  // state scan rfid
  if (!this->rfidScanned && this->rfidScanner->ready())
  {
    Serial.println("!this->rfidScanned && this->rfidScanner->ready()");
    this->actualRfid = this->rfidScanner->get();
    this->rfidScanned = true;
  }

  if (this->rfidScanned && !this->sendedSignal && this->bluetoothReciever->ready())
  {
    Serial.println("this->rfidScanned");
    this->sendData2Slave->doIt(NULL);
    //todo wann ist bereit?
    //  this->sendedSignal = this->bluetoothReciever->ready();

    String command = this->bluetoothReciever->get();
  }

  if (this->rfidScanned && this->sendedSignal && !this->positionXReached)
  {
    Serial.println("this->goToPositionX");
  }

  return false;
}
