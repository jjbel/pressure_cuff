#pragma once

#include "SerialReader.h"
#include <Arduino.h>

struct Reader {
  static constexpr uint64_t MAX_SIZE = 32;
  char buffer[MAX_SIZE];
  uint64_t length = 0;

  // Non-blocking serial read
  // https://forum.arduino.cc/t/serial-input-basics-updated/382007
  // cud even do while(read()){} so if many reach at once?

  void read(State &state) {
    if (Serial.available() <= 0) {
      return;
    }

    const char ch = Serial.read();

    if (ch == '\n') {
      // Serial.print("processing... ");
      // Serial.println((int)length);

      process(state);
      clear();
    } else if (length == MAX_SIZE) {
      Serial.println("warning: buffer overflow");
      clear();
    } else {
      buffer[length] = ch;
      length++;
    }
  }

  void process(State &state) {
    // Serial.print((int)length);
    // Serial.print(' ');
    // Serial.println(buffer[0]);

    if (length == 1) {
      // Serial.print(int(buffer[0]));
      if (buffer[0] == 48) {
        state.off();
      } else if (buffer[0] == 49) {
        state.on();
      }
    } else if (length == 2) {
      Serial.print("got: ");
      Serial.print(int(buffer[0]));
      Serial.print(' ');
      Serial.println(int(buffer[1]));

      // TODO put a mapping function or a LUT instead of float
      state.out_params[buffer[0]] = float(buffer[1] - 48) * 1000;
      // Serial.println(state.out_params[buffer[0] - 48]);
      Serial.println(state.out_params[3]);
    }
  }

  void clear() { length = 0; }
};
