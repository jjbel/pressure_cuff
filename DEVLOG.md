## `2025-01-01`, commit `36cbe84`

Got HX710b pressure sensor working.
Read raw sensor value using `HX711::read()`, then did manual calibration:

1. Using a T junction, connected the pump, sensor, and mercury section of the sphygmomanometer.

2. Used the smaller diameter tube from an IV set.

3. Took around 7 readings, and used desmos to do a linear regression between sensor reading and pressure in mm Hg measured on the sphygmomanometer.

4. **FIXME:** `HX711::read()` gives a 24 bit value as a `long`, from the ADC. But it seems to overflow at $2^{23} - 1 = 8388607$, suggesting it outputs a signed value? But this limits pressure measurement to around 130 mm Hg.

5. Noticed that Air Compressors for vehicle tyres, like [this Xiaomi product](https://www.mi.com/in/product/mi-portable-electric-air-compressor/) serve a similar purpose, with a pump, sensor, display and control loop.

6. Pump blows outward regardless of direction of rotation. To decrease pressure, use a solenoid valve. But the commonly available solenoid valves need $12V$ or $24V$ input, so use try a boost converter to convert $5V$ to $12V$. To toggle it on and off, connect it to the motor driver shield.

7. The cuff is a greater load than the mercury column of the sphygmomanometer, so the former needs running the motor faster. Motor doesn't seem to run at speeds lower than 0.5, possibly because of starting when load is connected.

## `2025-01-04`

### A. Boost Converter: Weird Behaviour

Intent: programatically toggle solenoid using motor shield to give or cut DC output. Use boost converter to give 12V to solenoid.

1. Involves: 5V power, M1,M2 terminals of motor shield, input, output and pot (potentiometer) of boost, multimeter

2. 5V power to input, output to multimeter: tune pot to make output 12V

3. When M1, M2 unloaded, multimeter shows 4.8V to 5V (expected)

4. When motor connected, M1 shows 3.3V (?)

5. At PWM=255, when boost connected, M2 shows 3V to 3.166V, and output shows 42V (? minus 42) at existing pot value. (here solenoid disconnected) (??)

6. At PWM=255, minimum pot gives 14.8V output (can't reach 12V), even though boost rated for min 3V. Possibly because motor shield has weird waveform. Should analyze with oscilloscope. (???)

7. At lower PWM and min pot, could tune output to 12V, but 8V to 9.5V when solenoid connected.
 At lower voltage, solenoid works but vibrates.

### B. Maintaining constant pressure with motor+solenoid

1. Can run motor at various speeds. Can only turn solenoid on or off, but can keep it in that state for various durations.

2. Aim: maintain pressure, at say setpoint=40 mm Hg.

3. With load (cuff connected) motor builds pressure at moderate speed, but solenoid drops pressure quickly. One fix: use tube adjustment knob, which matches outflow rate of solenoid with inflow rate of motor. But this has to be tuned manually for different pressures, so avoid.

4. Keeping the motor always running and opening the solenoid beyond 40 results in oscillations (sometimes with 1 or 2 subpeaks, and the solenoid triggering like a heartbeat). 

5. The oscillations have a max of only around 42, but min down to 30 because the motor is slow (half speed) and the solenoid drops pressure fast. This is with a 100ms delay each cycle

6. removing the delay and having the motor run at full speed: oscillations are shorter duration and more centred around 40 mm Hg. **Maybe delay could be reduced** say by reducing calls to setSpeed.

7. How to do PID control: motor speed is dense, but **solenoid state is binary**

8. Just turning the motor on and off and keeping the solenoid closed: results in quick increase to 42 and exponential decrease to 40 (because of leaks in the system?). This looks like capacitor charge and discharge cycles. Similar behaviour for setpoint=80 mm Hg. Could **use PID control on motor speed for this**

9. Final aim is not to maintain pressure but to cycle it. Leak with solenoid closed is too slow. Solenoid is needed for quickly reducing pressure, but it is too quick. Maybe could **open solenoid for only, say 10ms**

10. Did plumbing changes along the way: added another T junction, removed the sphygmomanometer and added back the pressure cuff. 1 port of solenoid for input, one for electronic pressure release, and one kept shut, but can be manually opened for pressure release. 
