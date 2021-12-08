#pragma once
#include "Consts.h"

template <class T> void saveToEEPROM(int addrs, const T& value);
template <class T> T readFromEEPROM(int addrs, T& value);
String readPlayerNameFromEEPROM(int addrs);
