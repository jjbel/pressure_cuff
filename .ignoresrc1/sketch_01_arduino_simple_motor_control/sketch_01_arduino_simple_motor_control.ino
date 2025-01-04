#include <AFMotor.h>

AF_DCMotor motor1(1);

int ramp_up_ms = 4000;
int steady_high_ms = 4000;
int ramp_down_ms = 4000;
int steady_low_ms = 4000;

float max_pressure = 50;

const auto t1 = ramp_up_ms;
const auto t2 = t1 + steady_high_ms;
const auto t3 = t2 + ramp_down_ms;
const auto t4 = t3 + steady_low_ms;

// 0 -ramp_up- t1 -steady_high- t2 -ramp_down- t3 -steady_low- t4

unsigned long start_ms = 0;
unsigned long current_ms = 0;


float lerp(float value, float from_min, float from_max) {
  return (value - from_min) / (from_max - from_min);
}

float lerp(float value, float from_min, float from_max, float to_min, float to_max) {
  return lerp(value, from_min, from_max) * (to_max - to_min) + to_min;
}

float ramp_speed() {
  current_ms = millis();
  const unsigned long elapsed_ms = current_ms - start_ms;

  if (elapsed_ms > t4) {
    start_ms = millis();
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

void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  // cud also use signed integers and declare var in if condn and use comma op
  float factor = 255 * ramp_speed();
  motor1.setSpeed(factor);
  motor1.run(FORWARD);
  Serial.println(factor);
}
