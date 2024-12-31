#include "HX711.h"
#include <AFMotor.h>
#include <Arduino.h>

#include "sensor_init.h"
#include "util.h"

HX711 sensor;

int frame = 0;
uint64_t time = 0;

AF_DCMotor motor = AF_DCMotor(1);

void setup() {
  Serial.begin(38400);
  sensor_init(sensor, A1, A0, 1.f);

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

void loop() {
  float factor = 255 * 0.25;
  motor.setSpeed(factor);
  motor.run(FORWARD);

  const float val = float(sensor.read());

  //   println(frame, /* millis(), */  +
  //  1316000 /* sensor.get_offset(), read() */);
//   println(val, val * 0.0000138368 + 18.38962, val * 0.0000138368 - 18.02885);
  println(val * 0.0000138368 + 18.38962, "mm Hg");
  frame++;
}
