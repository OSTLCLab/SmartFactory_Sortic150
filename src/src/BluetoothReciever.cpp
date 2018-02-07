#include <Arduino.h>
#include <BlootoothReciever.h>

bool BlootoothReciever::ready()
{
  return this->reciever.available() > 0;
}
String BlootoothReciever::get()
{
  return this->reciever.readStringUntil('\n');
}
