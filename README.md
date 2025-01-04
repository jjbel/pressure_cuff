A device to apply and release pressure in a cuff wrapped around a limb, to ease pain.


### How existing digital blood pressure monitors work

Motor pump increases pressure in cuff

When pressure = blood pressure, blood flow stops => detected by microphone / stethoscope => used to know at what pressure to take systolic/diastolic reading.
This is not needed for our device

Has a separate sensor to continuously measure pressure, connected with a T junction between the pump and the cuff.

### Our Device

Only increases and decreases pressure cyclically

There are leaks etc, so motor needs to keep running to maintain pressure
Possibly use PID control

## Parts

### HX710B pressure sensor

comes with a port for 2.5mm hose

https://robu.in/product/hx710b-air-pressure-0-40kpa-sensor-module/

https://www.electroschematics.com/pressure-sensor-guide/
    - uses: https://github.com/bogde/HX711

https://forum.arduino.cc/t/using-hx710b-pressure-sensor/648374/3

### Mitsumi R-14 A212: air pump

Currently unavailable :(

Can harvest existing BP devices for the pump

### XL6009 boost converter

To convert $5V$ from motor shield to $12V$ for solenoid valve.

https://amzn.in/d/8wb3pvw

Tutorial: https://youtu.be/ehxq8R4muGI

Explanation: https://www.monolithicpower.com/en/learning/mpscholar/power-electronics/dc-dc-converters/buck-boost-converters

### Screen

https://robu.in/product-category/microcontroller-development-board/arduino-boards/display-for-arduino/

https://www.google.com/search?q=arduino+screen+tutorial

https://en.wikipedia.org/wiki/TFT_LCD: enlightening

---

- Use processing, later use a display, and possibly a UI library
  either physics buttons or touchscreen

- Pressure relief valve with a solenoid
  Also one for user in emergency

- Powered by wall wart + rechargeable battery
  (newer arduino’s come with battery management system?)

- User interface: On, off, change program
  Program has ramp up time, max pressure steady time, ramp down time, no of cycles
