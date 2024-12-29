#pragma once

#include "HX711.h"
#include "util.h"

void sensor_init(HX711& sensor, byte dout_pin, byte sck_pin, float scale) {

  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT               - pin #A1
  // HX711.PD_SCK           - pin #A0
  sensor.begin(dout_pin, sck_pin);

  println("Before setting up the sensor:");
  println("read: \t\t", sensor.read()); // print a raw reading from the ADC

  // print the average of 20 readings from the ADC
  println("read average: \t\t", sensor.read_average(20));

  // print the average of 5 readings from the ADC
  // minus the tare weight (not set yet)
  println("get value: \t\t", sensor.get_value(5));

  // print the average of 5 readings from the ADC minus tare  weight (not set)
  // divided by the SCALE parameter (not set yet)
  println("get units: \t\t", sensor.get_units(5));

  // TODO manually calibrate
  // this value is obtained by calibrating the sensor
  // with known weights; see the README for details
  sensor.set_scale(scale);
  sensor.tare(); // reset the sensor to 0

  println("After setting up the sensor:");

  // print a raw reading from the ADC
  println("read: \t\t", sensor.read());

  // print the average of 20 readings from the ADC
  println("read average: \t\t", sensor.read_average(20));

  // print the average of 5 readings from the ADC
  // minus the tare weight, set with tare()
  println("get value: \t\t", sensor.get_value(5));

  // print the average of 5 readings from the ADC minus tare weight,
  // divided by the SCALE parameter set with set_scale
  println("get units: \t\t", sensor.get_units(5), 1);
}
