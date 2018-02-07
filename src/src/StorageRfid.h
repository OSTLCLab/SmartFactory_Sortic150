#include <Command.h>
#include <Sensor.h>

#ifndef storagerfid_h
#define storagerfid_h

class StorageRfid : public Command
{
public:
  StorageRfid(Sensor<uint8_t *> *rfidScanner,
              Command *run2Position,
              Command *sendData2Slave,
              Sensor<String> *bluetoothReciever) : rfidScanner{rfidScanner},
                                                   run2Position{run2Position},
                                                   sendData2Slave{sendData2Slave},
                                                   bluetoothReciever{bluetoothReciever} {}
  bool doIt(uint8_t *position);

private:
  Sensor<uint8_t *> *rfidScanner;
  Command *run2Position;
  Command *sendData2Slave;
  Sensor<String> *bluetoothReciever;
  uint8_t *actualRfid;
  bool rfidScanned{false};
  bool sendedSignal{false};
  bool positionXReached{false};
};

#endif
