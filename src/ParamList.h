#include "Button.h"
#include "Param.h"

struct ParamList {
  const int param_count = 3;

  Param params[3] = {{"Pmax", 90, 10, 10, 10, 120},
                     {"Hi s", 10, 10, 10, 10, 9999},
                     {"Lo s", 4, 10, 10, 10, 9999}};

  int index = 0;

  Button increase = Button(2);
  Button cycle = Button(3);
  Button decrease = Button(4);

  void update() {
    increase.update();
    cycle.update();
    decrease.update();

    if (increase.is_pressed()) {
      params[index].increment();
    }

    if (decrease.is_pressed()) {
      Serial.println("dec");
      params[index].decrement();
    }

    if (cycle.is_pressed()) {
      index = (index + 1) % param_count;
    }
  }
};
