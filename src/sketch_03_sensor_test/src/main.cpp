#include "HX711.h"
#include <Arduino.h>

#include "sensor_init.h"
#include "util.h"

HX711 sensor;

int frame = 0;
uint64_t time = 0;

void setup() {
  Serial.begin(38400);
  sensor_init(sensor, A1, A0, 2280.f);

  frame = 0;
  time = millis();
}

float read() {
  // TODO delay(0) in HX711 code causes a long wait?
  // also HX711 uses int division instead of float division
  //   so manually call read()
  return (float(sensor.read()) - float(sensor.get_offset())) /
         float(sensor.get_scale());

  // TODO reading increases steadily, but sometimes resets
  //   maybe needs pullup resistor?
  //   println(frame, millis(), read());
}

void loop() {
  println(frame, millis(), read());
  frame++;
}
