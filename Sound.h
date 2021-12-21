#pragma once

class Sound{
  bool enabled;
  static Sound *instance;

  Sound();
  public:
    static Sound *getInstance();
    void setSound(bool value);
    bool getSound();
    void playSound(const int freq,const int duration);
};
