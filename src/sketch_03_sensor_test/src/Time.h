#pragma once

#include <Arduino.h>

struct Time {
  uint64_t start_ms = 0;
  uint64_t current_ms = 0;
};