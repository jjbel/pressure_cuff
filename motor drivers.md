L298N has a drop of ~2V
so outputs 2.7V (no load)
=> can't power boost converter (rated min=5V)

just put a boost converter between the power supply and the shield!!

read ALL these:

https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/

https://www.haoyuelectronics.com/Attachment/XL6009/XL6009-DC-DC-Converter-Datasheet.pdf

https://forum.arduino.cc/t/voltage-drop-on-l298n/668395/2
https://www.google.com/search?q=modern+alternative+to+l298n+motor+driver
https://forum.arduino.cc/t/what-is-the-alternative-for-l298n-and-why-should-i-use-it/954480
https://www.pololu.com/product/713
https://www.pololu.com/file/0J86/TB6612FNG.pdf
https://robu.in/product-category/dc-motors/motor-drivers/
https://www.amazon.in/s?k=motor+driver&crid=3MRW4LGVUPWU5&sprefix=motor+driver%2Caps%2C262&ref=nb_sb_noss_2
https://www.amazon.in/Tb6612Fng-Driver-Module-Arduino-Microcontroller/dp/B07ZFB6MTZ
https://www.amazon.in/Double-BTS7960-H-Bridge-High-Power-Stepper/dp/B08WKK9GQC
https://www.google.com/search?client=firefox-b-d&q=motor+driver+mosfet
https://electronics.stackexchange.com/questions/102819/driving-dc-motors-with-mosfets-and-a-microcontroller
https://www.google.com/search?client=firefox-b-d&q=TB661fng+voltage+drop
https://forum.arduino.cc/t/is-there-a-loss-in-voltage-to-motor-through-motor-driver/602136/2


UPDATE
Just use 6V power supply and boost for solenoid. Motor gets 5V (rated 6)
Or use 12V power supply and buck for Motor
Either way, arduino thru the barrel jack (make a male barrel jack or make it(get a female)) 

Separately can get mosfet motor driver

If sticking with 5V power supply, put a boost before it, since it can handle 4A


