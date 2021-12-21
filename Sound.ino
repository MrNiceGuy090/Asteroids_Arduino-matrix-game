#include "Sound.h"

Sound *Sound::instance = 0;

Sound::Sound(){
  enabled = readFromEEPROM<bool>(soundOnOffAddrs);
}

Sound* Sound::getInstance(){
  if(!Sound::instance){
    Sound::instance = new Sound;
  }
  return Sound::instance;
}

void Sound::setSound(bool value){
  Serial.println(value);
  enabled = value;
  saveToEEPROM<bool>(soundOnOffAddrs, enabled);
}

bool Sound::getSound(){
  return enabled;
}

void Sound::playSound(const int freq,const int duration){
  if(enabled){
    tone(buzzerPin, freq, duration);
  }
}
