#pragma once

#include <AFMotor.h>
#include <Arduino.h>

#include "util.h"

struct State {
  AF_DCMotor motor = AF_DCMotor(1);

  static constexpr uint64_t PARAM_COUNT = 6;
  char in_params[PARAM_COUNT];

  bool running = false;
  float out_params[PARAM_COUNT] = {
      40,   // max P (mm Hg)
      0.3,  // session duration (min)
      4000, // ramp_up_ms
      4000, // steady_high_ms
      4000, // ramp_down_ms
      2000  // steady_low_ms
  };

  void deflate() {
    Serial.println("deflating...");
    // release solenoid
    // TODO
  }

  void off() {
    running = false;
    motor.run(RELEASE);
    deflate();
  }

  void on() {
    running = true;
    Serial.println("on");
  }

  float ramp_speed(Time &time) {
    // 0 -ramp_up- t1 -steady_high- t2 -ramp_down- t3 -steady_low- t4

    const int t1 = out_params[2];
    const int t2 = t1 + out_params[3];
    const int t3 = t2 + out_params[4];
    const int t4 = t3 + out_params[5];

    time.current_ms = millis();
    const uint64_t elapsed_ms = time.current_ms - time.start_ms;

    if (elapsed_ms > t4) {
      time.start_ms = millis();
      return 0;
    } else if (elapsed_ms > t3) {
      return 0;
    } else if (elapsed_ms > t2) {
      return 1 - lerp(elapsed_ms, t2, t3);
    } else if (elapsed_ms > t1) {
      return 1;
    } else {
      return lerp(elapsed_ms, 0, t1);
    }
  }
};
