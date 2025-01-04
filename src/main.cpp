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

void loop() {
  const float val = float(sensor.read());
  const float pressure = val * 0.0000138368 + 18.38962;
  println(pressure, "mm Hg");

  if (pressure < SETPOINT) {
    float factor = 255 * (4.0 / 8);
    motor.setSpeed(factor);
    motor.run(FORWARD);
  } else {
    motor.run(RELEASE);
    println("done");
    delay(2000);
  }

  solenoid.setSpeed(255);
  solenoid.run(FORWARD);

//   delay(500);

  frame++;
}

// with bp app and not cuff, pressure rises quickly, so ran at 3/8 speed with 2s
// lag with cuff, pressure rises slowly
