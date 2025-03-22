#include "HX711.h"
#include <Arduino.h>

#include "Display.h"
#include "ParamList.h"
#include "sensor_init.h"
#include "util.h"

// const float SETPOINT = 120; // mm Hg
const float DETECTION_FACTOR = 0.95;
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

constexpr int display_width = 128;
constexpr int display_height = 64;
constexpr int line_height = 17;
constexpr int char_width = 12; // 10px + 2px spacing
constexpr int arrow_width = 8;

void draw(float pressure) {
  display.display.clearDisplay();

  // https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
  // height of screen - 64px - roughly fits 4 lines
  // take line height = 17

  // display.display.drawRect(0, param_list.index * line_height,
  //                          display.display.width(), line_height, WHITE);
  const int y_start = param_list.index * line_height;
  const int y_mid = y_start + line_height / 2;
  display.display.drawLine(0, y_start, arrow_width, y_mid, WHITE);
  display.display.drawLine(arrow_width, y_mid, 0, y_start + line_height, WHITE);

  for (int i = 0; i < param_list.param_count; i++) {
    display.display.setCursor(arrow_width + 5, i * line_height);

    const Param &param = param_list.params[i];
    display.display.print(param.name);
    display.display.print(" ");

    // max 4 digits of number
    display.display.setCursor(display_width - 4 * char_width,
                              i * line_height + 1);
    display.display.print((int)param.value);
  }

  display.display.setCursor(arrow_width + 5, 3 * line_height);
  display.display.print("P=");
  display.display.print((int)pressure);

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

  if (count > param_list.params[1].value * 1000) {
    Serial.println("Deflate!");
    analogWrite(solenoid_pin, 0);

    delay(param_list.params[2].value * 1000);

    // TODO shd be 2 modes..high and low
    display.display.setCursor(0, 3 * line_height);
    display.display.print("P = ");
    display.display.print(0);

    count = 0;
    start = millis();
  } else {
    analogWrite(solenoid_pin, 255);
  }
  frame++;

  delay(100);
}
