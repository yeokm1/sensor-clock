sensor-clock
============

A clock composed of an Arduino, temperature, humidity and pressure sensors.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/front-day-temp-hum.jpg)

Front. The temperature and humidity reading will switch to the pressure reading and approximate altitude at regular intervals. The DHT22 (white sensor) is placed externally to prevent heat from the Arduino from affecting the reading.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/back.jpg)

Backup Lithium battery and charger placed at the back due to space constraint. Those are erasers at the bottom to act as bumpers.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/internals.jpg)

Just for the curious.


<b>Main Parts used:</b>  

1. Arduino Mega R3  (An Uno or other boards will work too)  
2. LCM1602C LCD (from official Arduino Starter kit)  
3. DHT22 temperature and humidity sensor  
4. BMP180 Pressure and temperature sensor. (temperature unused)  
5. Chronodot v2.1 (based on DS3231 temperature compensated RTC crystal)  


Optional:  

1. Half breadboard  
2. 2000 mAh Lithium battery  
3. Lipo charger  



![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/sensor clock schematic.png)

A Fritzing schematic. This schematic only represents the logical connections I made. The physical connections differs due to space issues. As Fritzing does not have DHT22, BMP180 and DS3231 chronodot, I modified some existing and downloaded designs.

<b>Things to take note before modifying my code/design:</b>  

1. DHT22 sensor needs at least 2 seconds to take a reading.  

2. BMP180 needs to take a temperature reading (even if you do not use it) before taking a pressure reading. Both readings need a delay of 5ms each.

3. BMP180 requires a 3.3V input.

4. One is supposed to add a pull-up resistor for the i2c bus SCL and SDA lines if more than one device (BMP180 and Chronodot) uses them. I tried without the resistor and it seems to work fine.


References and libraries:  

1. Using Arduino LCD without potentiometer.  
(http://engineeringlearning.blogspot.sg/2013/10/interfacing-lcd-without-potentiometer.html)

2. Get day of week from date.  
(http://stackoverflow.com/a/21235587)

2. Temperature and Humidity Icons  
(http://www.instructables.com/id/Clock-with-termometer-using-Arduino-i2c-16x2-lcd-D/)  

3. Adafruit DHT sensor library  
(https://github.com/adafruit/DHT-sensor-library)

4. Adafruit DS1307 RTC library (Works for DS3231 too. I used this because of a neat feature of setting the clock to compilation time after every upload)  
(https://github.com/adafruit/RTClib)

5. Sparkfun BMP180 library  
(https://github.com/sparkfun/BMP180_Breakout)

7. New Liquid Crystal library (for supposedly better LCD performance)  
(https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home)



