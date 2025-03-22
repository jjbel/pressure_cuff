#include "Button.h"
#include "Param.h"
#include "util.h"

Button button1(2);
Button button2(3);
Button button3(4);
Param param_pressure{"pressure", 0, 10, 10, 10, 120};

void setup() { Serial.begin(38400); }

void loop() {
  if (button1.is_pressed()) {
    param_pressure.increment();
    // Serial.println(param_pressure.value);
  }

  button1.update();
  button2.update();
  button3.update();

  println(button1.is_pressed(), button2.is_pressed(), button3.is_pressed());

  delay(100);
}
