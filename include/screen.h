#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Arduino.h>

#include "config.h"
#include "helper.h"
#include "settings.h"

class Screen {
 private:
  Adafruit_PCD8544 _display =
      Adafruit_PCD8544(LCD_CLK, LCD_DIN, LCD_DC, LCD_CS, LCD_RST);
  Settings* _settings;

  void bootScreen();
  void mainMenu();
  void mainScreen(String* status);
  void powerMenu();
  void modeMenu();
  void timeMenu();
  void printCentered(uint16_t y, String text);

 public:
  void begin(Settings* settings);
  void tic(String* status);
};

struct MenuItem {
  String name;
  void (Screen::*callback)();
};

#endif