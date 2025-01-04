### `2025-01-01`, commit `36cbe84`

Got HX710b pressure sensor working.
Read raw sensor value using `HX711::read()`, then did manual calibration:

1. Using a T junction, connected the pump, sensor, and mercury section of the sphygmomanometer.

2. Used the smaller diameter tube from an IV set.

3. Took around 7 readings, and used desmos to do a linear regression between sensor reading and pressure in mm Hg measured on the sphygmomanometer.

4. **FIXME:** `HX711::read()` gives a 24 bit value as a `long`, from the ADC. But it seems to overflow at $2^{23} - 1 = 8388607$, suggesting it outputs a signed value? But this limits pressure measurement to around 130 mm Hg.

5. Noticed that Air Compressors for vehicle tyres, like [this Xiaomi product](https://www.mi.com/in/product/mi-portable-electric-air-compressor/) serve a similar purpose, with a pump, sensor, display and control loop.

6. Pump blows outward regardless of direction of rotation. To decrease pressure, use a solenoid valve. But the commonly available solenoid valves need $12V$ or $24V$ input, so use try a boost converter to convert $5V$ to $12V$. To toggle it on and off, connect it to the motor driver shield.
