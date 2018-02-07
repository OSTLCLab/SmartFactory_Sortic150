#include <RfidScanner.h>

bool RfidScanner::ready()
{
    return this->scanner.PICC_IsNewCardPresent();
};

uint8_t *RfidScanner::get()
{
    byte bufferSize = 18;
    byte blockAddr = 0;
    uint8_t *value = new uint8_t[bufferSize];
    this->scanner.MIFARE_Read(blockAddr, value, &bufferSize);

    return value;
}
