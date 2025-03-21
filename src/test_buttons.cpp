#include "Button.h"
#include "Param.h"

Button button(2);
Param param_pressure{"pressure", 0, 10, 10, 10, 120};

void setup() { Serial.begin(38400); }

void loop() {
  if (button.is_pressed()) {
    param_pressure.increment();
    Serial.println(param_pressure.value);
  }

  button.update();

  delay(100);
}
