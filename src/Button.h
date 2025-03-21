#include "Arduino.h"

struct Button {
  int pin;
  bool current = false;
  bool prev = false;

  Button(int pin) : pin(pin) { pinMode(pin, INPUT_PULLUP); }

  void update() {
    prev = current;
    // input pullup so invert
    current = !digitalRead(pin);
  }

  bool is_pressed() const { return current && !prev; }
  //   void is_released() { return !current && prev; }
};
