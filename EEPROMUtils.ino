#include "EEPROMUtils.h"
#include "Arduino.h"

template <class T> void saveToEEPROM(int addrs, const T& value)
{
    EEPROM.put(addrs, value);
}

template <class T> T readFromEEPROM(int addrs)
{
    T value;
    value = EEPROM.get(addrs, value);
    return value;
}

String readPlayerNameFromEEPROM(int addrs){
  String name = "";
  for(int i = addrs; i < addrs + maxLengthName; i++){
    char c = readFromEEPROM<char>(i);    
    if(c != 0)name += readFromEEPROM<char>(i);
  }
  return name;
}
