#include "HX711.h"
#include <Arduino.h>

#include "Display.h"
#include "ParamList.h"
#include "sensor_init.h"
#include "util.h"

// const float SETPOINT = 120; // mm Hg
const float DETECTION_FACTOR = 0.95;
const float HOLD_SECONDS = 6000;
const float DEFLATE_MS = 1000;

const int motor_pin = 5;
const int solenoid_pin = 6;

HX711 sensor;

int frame = 0;

Display display;

ParamList param_list;

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

void draw(float pressure) {
  display.display.clearDisplay();
  display.display.setCursor(0, 0);

  for (int i = 0; i < param_list.param_count; i++) {
    const Param &param = param_list.params[i];
    display.display.print(param.name);
    display.display.print(" ");
    display.display.println(param.value);
  }

  display.display.print("P=");
  display.display.println(pressure);

  display.display.display(); // Show initial text
}

void loop() {
  const float val = float(sensor.read());
  const float pressure = val * 0.0000138368 + 18.38962;

  param_list.update();
  draw(pressure);

  if (pressure < param_list.params[0].value * DETECTION_FACTOR) {
    count = 0;
    start = millis();
  } else {
    count = millis() - start;
  }

  if (pressure < param_list.params[0].value) {
    float factor = 1.0;
    analogWrite(motor_pin, 127 * factor);

  } else {
    analogWrite(motor_pin, 0);
  }

  println(pressure, "mm Hg", count);

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
