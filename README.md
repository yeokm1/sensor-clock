sensor-clock
============

A clock composed of an Arduino, temperature, humidity and pressure sensors.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/front-day-temp-hum.jpg)

Front. The temperature and humidity reading will switch to the pressure reading and approximate altitude at regular intervals. The DHT22 (white sensor) is placed externally to prevent heat from the Arduino from affecting the reading.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/back.jpg)

Contains the backup Lithium battery and charger.

![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/internals.jpg)

Just for the curious.


Main Parts used:  
1. Arduino Mega R3  
2. LCM1602C LCD (from official Arduino Starter kit)  
3. DHT22 temperature and humidity sensor  
4. BMP180 Pressure and temperature sensor. (temperature unused)  
5. Chronodot v2.1 (based on DS3231 temperature compensated RTC crystal)  


Optional:  
1. Half breadboard  
2. 2000 mAh Lithium battery  
3. Lipo charger  



![Screen](https://raw.github.com/yeokm1/sensor-clock/master/misc/sensor clock schematic.png)

A Fritzing schematic. This picture only represents the logical connections I made. The physical connections differs due to space issues. As Fritzing does not have DHT22, BMP180 and DS3231 chronodot, I modified some existing and downloaded designs.
