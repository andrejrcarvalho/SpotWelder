#include "settings.h"

void Settings::loadFromEEPROM(){
  pulseTime = 100;
  pulsePower = 75;
  mode = MODE_PULSE;
}

