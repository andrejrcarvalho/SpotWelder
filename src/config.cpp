
#include "config.h"

void setupPinModes(){
  pinMode(TRIAC_PIN, OUTPUT);
  digitalWrite(TRIAC_PIN, LOW);
  pinMode(ZEROCORSS_PIN, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(PEDAL_SW, INPUT_PULLUP);
}