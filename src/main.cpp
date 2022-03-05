#include <Arduino.h>
#include <limits.h>

#include "config.h"
#include "screen.h"
#include "settings.h"

Settings settings;
Screen screen;

unsigned long timeCounter = ULONG_MAX;
bool zeroCross = 0;
short lastPedalStatus = HIGH;
String status = "Ready";

void zeroCrossing();
void pulse();
void setSelectedText(bool value);
int encoderSelect(int step);

void setup() {
  Serial.begin(9600);

  // Setup PIN
  setupPinModes();

  // Stop interrupts for till make the settings
  noInterrupts();

  // Setup Timer
  TCCR2A = 0;
  TCCR2A |= (1 << WGM21);  // set the CTC (Clear Timer on Compare) mode for
                           // timer0 with OCRA as top
  TCCR2B = 0;
  TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);  // Set prescaler to 1024
  TIMSK2 |= (1 << OCIE2A);  // activate timer compare match interrupt for OCR0A
                            // and OCR0B

  // Setup Interrupt to detect zero crossing
  attachInterrupt(digitalPinToInterrupt(ZEROCORSS_PIN), zeroCrossing, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), encoderInterrupt, CHANGE);

  // Activate again the interrupt
  interrupts();

  settings.loadFromEEPROM();
  screen.begin(&settings);

  Serial.println("STATING....");
}

void loop() {
  short pedalStatus = digitalRead(PEDAL_SW);
  if (pedalStatus != lastPedalStatus) {
    if (pedalStatus == LOW) {
      pulse();
      status = "Pulse";
    } else {
      status = "Ready";
    }
    lastPedalStatus = pedalStatus;
  }

  screen.tic(&status);
}

void pulse() {
  Serial.print("Pulse(ms): ");
  Serial.println(settings.pulseTime);
  Serial.print("Power: ");
  Serial.println(settings.pulsePower);
  timeCounter = 0;
}

// external interrupt, occurs after every zero crossing point
void zeroCrossing() {
  if ((settings.mode == MODE_TIME_PULSE && timeCounter < settings.pulseTime) ||
      (settings.mode == MODE_PULSE && lastPedalStatus == LOW)) {
    TCNT2 = 0;  // clear timer2 counter
    OCR2A = map(settings.pulsePower, 100, 0, 10,
                155);  // set OCR2B value between 10 and 156 (0.64ms - 9.98ms)
                       // according to the potentiometer value
    zeroCross = 1;     // set zero crossing point flag to 1
    timeCounter += 10;
  }
  digitalWrite(TRIAC_PIN, LOW);
}

ISR(TIMER2_COMPA_vect) {  // compare match interrupt for OCR0A
  if (zeroCross == 1) {
    digitalWrite(TRIAC_PIN, HIGH);
    zeroCross = 0;
  }
}