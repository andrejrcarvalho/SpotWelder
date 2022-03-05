#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define TRIAC_PIN 4
#define ZEROCORSS_PIN 2 // Needs to be a interrupt pin

#define LCD_CLK 8 
#define LCD_DIN 9
#define LCD_DC 10
#define LCD_CS 11
#define LCD_RST 12

#define ENCODER_SW 6
#define ENCODER_CLK 3
#define ENCODER_DT 5

#define PEDAL_SW 7

void setupPinModes();

#endif