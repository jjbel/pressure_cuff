#include "HX711.h"
#include <Arduino.h>

#include "Display.h"
#include "sensor_init.h"
#include "util.h"

const float SETPOINT = 120; // mm Hg
const float DETECTION_FACTOR = 0.95;
const float HOLD_SECONDS = 6000;
const float DEFLATE_MS = 1000;

const int motor_pin = 5;
const int solenoid_pin = 6;

HX711 sensor;

int frame = 0;

Display display;

void setup() {
  Serial.begin(38400);
  sensor_init(sensor, /* dout */ A1, /* sck */ A0);
  display.init();

  frame = 0;
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

int count = 0;
int start = millis();

void loop() {
  const float val = float(sensor.read());
  const float pressure = val * 0.0000138368 + 18.38962;

  if (pressure < SETPOINT * DETECTION_FACTOR) {
    count = 0;
    start = millis();
  } else {
    count = millis() - start;
  }

  if (pressure < SETPOINT) {
    float factor = 1.0;
    analogWrite(motor_pin, 127 * factor);

  } else {
    analogWrite(motor_pin, 0);
  }

  println(pressure, "mm Hg", count);
  display.draw(pressure);

  if (count > HOLD_SECONDS * 1000) {
    Serial.println("Deflate!");
    analogWrite(solenoid_pin, 0);

    delay(DEFLATE_MS);
    count = 0;
    start = millis();
  } else {
    analogWrite(solenoid_pin, 255);
  }
  frame++;

  delay(100);
}
