#include <Arduino.h>
#include "helper.h"
#include "config.h"

short buttonLastState = HIGH;
unsigned long buttonStateTime = 0;

long readButtonPressTime() {
  short actualState = digitalRead(ENCODER_SW);
  unsigned long pressedTime = 0;
  if(actualState != buttonLastState){
    if(actualState == LOW && buttonLastState == HIGH){
      buttonStateTime = millis();
    }else if (actualState == HIGH && buttonLastState == LOW)
    {
      pressedTime = millis()-buttonStateTime;
    }
    buttonLastState = actualState;
  }
  return pressedTime;
}

short encoderSpin = 0;
int readEncoder(int step)
{
  step = step*encoderSpin;
  encoderSpin = 0;

  return step;
}

int lastEncoderPosition = 0;
void encoderInterrupt(){
  int position = digitalRead(ENCODER_CLK);
  if (position != lastEncoderPosition)
  {
    if (digitalRead(ENCODER_DT) != position)
    {
     encoderSpin = 1;
    }
    else
    {
      encoderSpin = -1;
    }
  }
  lastEncoderPosition = position;
}