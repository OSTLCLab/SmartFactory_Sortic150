#include <Arduino.h>

#ifndef command_h
#define command_h

class Command
{
public:
  virtual bool doIt(uint8_t *command) = 0;
};

#endif
