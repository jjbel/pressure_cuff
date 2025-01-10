#include "HX711.h"
#include <AFMotor.h>
#include <Arduino.h>

#include "sensor_init.h"
#include "util.h"

HX711 sensor;

int frame = 0;
uint64_t time = 0;

AF_DCMotor motor = AF_DCMotor(1);

AF_DCMotor solenoid = AF_DCMotor(2);

void setup() {
  Serial.begin(38400);
  sensor_init(sensor, /* dout */ A1, /* sck */ A0);

  frame = 0;
  time = millis();

  delay(3000);
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

const float SETPOINT = 40; // mm Hg

int count = 0;
int start = millis();

void loop() {
  const float val = float(sensor.read());
  const float pressure = val * 0.0000138368 + 18.38962;

  if (pressure < 78) {
    count = 0;
    start = millis();
  } else {
    count = millis() - start;
  }

  if (pressure < 80) {
    float factor = 1.0;
    motor.setSpeed(255 * factor);
    motor.run(FORWARD);
  } else {
    motor.run(RELEASE);
  }

  println(pressure, "mm Hg", count);

  if (count > 6000) {
    Serial.println("Bam!");
    solenoid.run(RELEASE);
    delay(1000);
    count = 0;
    start = millis();
  } else {
    solenoid.setSpeed(255);
    solenoid.run(FORWARD);
  }
  frame++;
}
