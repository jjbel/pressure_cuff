#include "HX711.h"
#include <Arduino.h>

void print_space(auto &&arg) {
  Serial.print(arg);
  Serial.print(' ');
}

template <typename... Args> void println(Args &&...args) {
  (print_space(args), ...);
  Serial.println();
}

HX711 scale;

int frame = 0;
uint64_t time = 0;

void setup() {
  Serial.begin(38400);
  Serial.println("HX710B Demo with HX711 Library");
  Serial.println("Initializing the scale");

  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT               - pin #A1
  // HX711.PD_SCK           - pin #A0
  scale.begin(A1, A0);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(
      scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(
      scale.get_value(5)); // print the average of 5 readings from the ADC minus
                           // the tare weight (not set yet)
  Serial.print("get units: \t\t");
  // print the average of 5 readings from the ADC minus tare  weight (not set)
  // divided by the SCALE parameter (not set yet)
  Serial.println(scale.get_units(5), 1);

  scale.set_scale(2280.f); // this value is obtained by calibrating the scale
                           // with known weights; see the README for details
  scale.tare();            // reset the scale to 0

  Serial.println("After setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read()); // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(
      scale.read_average(20)); // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(
      scale.get_value(5)); // print the average of 5 readings from the ADC minus
                           // the tare weight, set with tare()
  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1); // print the average of 5 readings from
                                         // the ADC minus tare weight, divided
  // by the SCALE parameter set with set_scale
  Serial.println("Readings:");

  frame = 0;
  time = millis();
}

float read() {
  // TODO delay(0) in HX711 code causes a long wait?
  // also HX711 uses int division instead of float division
  //   so manually call read()
  return (float(scale.read()) - float(scale.get_offset())) /
         float(scale.get_scale());
}

void loop() {

  // TODO reading increases steadily, but sometimes resets
  //   maybe needs pullup resistor?

  //   const auto val = scale.get_units(30);
  //   Serial.println(val /* / millis() * 1000 */, 1);

  //   if (frame % 20 == 0) {
  //     const uint64_t new_time = millis();
  //     println(val, frame, float(new_time - time),
  //             float(frame) / float(new_time - time));

  //     frame = 0;
  //     time = new_time;
  //   }
  println(frame, millis(), read());

  //   Serial.print("one reading:\t");
  //   Serial.print(scale.get_units(), 1);
  //   Serial.print("\t| average:\t");
  //   Serial.println(scale.get_units(10), 1);
  //   scale.power_down(); // put the ADC in sleep mode
  //   delay(100);
  //   scale.power_up();
  frame++;
}
