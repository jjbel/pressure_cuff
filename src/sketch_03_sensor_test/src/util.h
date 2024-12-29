#pragma once

#include <Arduino.h>

void print_space(auto &&arg) {
  Serial.print(arg);
  Serial.print(' ');
}

template <typename... Args> void println(Args &&...args) {
  (print_space(args), ...);
  Serial.println();
}

float lerp(float value, float from_min, float from_max) {
  return (value - from_min) / (from_max - from_min);
}

float lerp(float value, float from_min, float from_max, float to_min,
           float to_max) {
  return lerp(value, from_min, from_max) * (to_max - to_min) + to_min;
}
