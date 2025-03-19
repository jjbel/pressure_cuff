#include <Arduino.h>

#define enA 5
#define enB 6

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  Serial.begin(38400);

  //   TODO why measuring 0.7-0.8V on motor driver terminals for motor block
}

void loop() {
  Serial.println("Hello");

  analogWrite(enA, 255);
  analogWrite(enB, 255);
  delay(2000);
//   analogWrite(enA, 0);
//   analogWrite(enB, 0);
//   delay(2000);
}
