#pragma once

#include "HX711.h"
#include "util.h"

void sensor_init(HX711 &sensor, byte dout_pin, byte sck_pin) {
  // parameter "gain" is ommited; the default value 128 is used by the library
  sensor.begin(dout_pin, sck_pin);

  sensor.set_scale(/* scale */ 1.f);
  sensor.tare(); // TODO needed? | reset the sensor to 0
}
