#include "screen.h"

void Screen::begin(Settings* settings) {
  _settings = settings;
  _display.begin();
  _display.setContrast(0);
  _display.clearDisplay();
  bootScreen();
}

void Screen::bootScreen() {
  _display.setTextSize(2);
  _display.setTextColor(BLACK, WHITE);
  _display.setCursor(10, 10);
  _display.print("HELLO!");
  _display.display();
  _display.clearDisplay();
  delay(1000);
}

void Screen::tic(String* status) {
  long pressedTime = readButtonPressTime();
  if (pressedTime > 50) {
    mainMenu();
  } else {
    mainScreen(status);
  }
}

void Screen::mainMenu() {
  MenuItem items[] = {{"> Power", &Screen::powerMenu},
                      {"> Time", &Screen::timeMenu},
                      {"> Mode", &Screen::modeMenu},
                      {"< Back", 0}};

  short menuLength = sizeof(items) / sizeof(MenuItem);
  short selector = 0;
  bool drawScreen = true;

  while (1) {
    short step = readEncoder(1);
    if (selector + step <= menuLength - 1 && selector + step >= 0)
      selector += step;

    if (readButtonPressTime() > 50) {
      if (items[selector].callback == 0) {
        break;
      } else {
        drawScreen = true;
        (this->*items[selector].callback)();
      }
    }

    if (step == 0 && !drawScreen) continue;
    drawScreen = false;

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(BLACK, WHITE);
    printCentered(0, "SETTINGS");
    _display.drawLine(0, 8, 84, 8, BLACK);
    for (short i = 0; i < menuLength; i++) {
      if (selector == i) {
        _display.setTextColor(WHITE, BLACK);
      } else {
        _display.setTextColor(BLACK, WHITE);
      }
      printCentered(10 + i * 10, items[i].name);
    }
    _display.display();
  }
}

void Screen::mainScreen(String* status) {
  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(BLACK, WHITE);
  _display.setCursor(3, 0);
  _display.print("MODE:");
  _display.print(_settings->mode == MODE_PULSE ? "Pulse" : "Timed");

  _display.drawLine(0, 8, 84, 8, BLACK);

  _display.setCursor(10, 15);
  _display.print("POWER");
  _display.setCursor(13, 23);
  _display.print(String(_settings->pulsePower) + "%");

  _display.setCursor(50, 15);
  _display.print("TIME");
  _display.setCursor(50, 23);
  if (_settings->mode == MODE_TIME_PULSE) {
    _display.print(String(_settings->pulseTime) + "ms");
  } else {
    _display.print("----");
  }

  _display.setCursor(3, 40);
  _display.setTextSize(1);
  _display.print("Status:" + *status);
  _display.display();
}

void Screen::powerMenu() {
  int power = _settings->pulsePower;
  int tmpPower = 0;

  while (readButtonPressTime() < 50) {
    int step = readEncoder(5);
    if ((step + power) >= 5 && (step + power) <= 100) power += step;

    if (step == 0 && tmpPower == power) continue;

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(BLACK, WHITE);
    printCentered(0, "POWER");
    _display.drawLine(0, 8, 84, 8, BLACK);

    _display.setTextSize(2);
    _display.setTextColor(BLACK, WHITE);
    printCentered(25, String(power) + "%");
    _display.display();
    tmpPower = power;
  }
  _settings->pulsePower = power;
}

void Screen::timeMenu() {
  unsigned long time = _settings->pulseTime;
  unsigned long tmpTime = 0;

  while (readButtonPressTime() < 50) {
    int step = readEncoder(10);
    if ((step + time) >= 10 && (step + time) <= 1000) time += step;

    if (step == 0 && tmpTime == time) continue;

    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(BLACK, WHITE);
    printCentered(0, "TIME");
    _display.drawLine(0, 8, 84, 8, BLACK);

    _display.setTextSize(2);
    _display.setTextColor(BLACK, WHITE);
    _display.setCursor(25, 25);
    printCentered(25, String(time));
    _display.setTextSize(1);
    _display.print("ms");
    _display.display();
    tmpTime = time;
  }
  _settings->pulseTime = time;
}

void Screen::modeMenu() {
  MenuItem items[] = {{"> Timed", 0}, {"> Pulse", 0}};
  short menuLength = sizeof(items) / sizeof(MenuItem);

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(BLACK, WHITE);
  printCentered(0, "MODE");
  _display.drawLine(0, 8, 84, 8, BLACK);

  short tmpMode = _settings->mode;
  bool drawScreen = true;
  while (readButtonPressTime() < 50) {
    int step = readEncoder(1);
    if ((step + tmpMode) >= 0 && (step + tmpMode) < menuLength) tmpMode += step;

    if (step == 0 && !drawScreen) continue;
    drawScreen = false;

    for (short i = 0; i < menuLength; i++) {
      if (tmpMode == i) {
        _display.setTextColor(WHITE, BLACK);
      } else {
        _display.setTextColor(BLACK, WHITE);
      }
      printCentered(10 + i * 10, items[i].name);
    }
    _display.display();
  }
  _settings->mode = tmpMode;
}

void Screen::printCentered(uint16_t y, String text) {
  uint8_t center = _display.width() / 2;
  uint16_t fontWidth = 0;
  _display.getTextBounds(text, 0, 0, 0, 0, &fontWidth, 0);

  _display.setCursor(center - (fontWidth / 2), y);
  _display.print(text);
}