# ColorClock
Simple Arduino clock that uses colors to approx. time.

Not everyone's vision is perfect. Sometimes in the middle of the night you want to know what the time is. Even an 
approximate time is better than nothing.

Created a simple clock that uses colors to denote the hour and minutes ( in 5 minute increments ). As many folks
can see colors without their glasses, it's an easy way to check to time before rolling over back to sleep.

Many not work for everyone, but it works for us :)

The below is the color and their corresponding Hours/Minutes. Each Minute hour corresponds to a 5 min time period. 
( We are not looking for super accuracy here after all ) 
Most of the time we wake up in the 5-7 range, so the colors are arranged for the reds/oranges of sunrise to be
in this range. Or, to be used as a warning to indicate sleep is almost over :)

| Hour:  | Minutes: | Color:    |
| :----: | :----:   | :----:    |
| 00     |  0-4     |   Cyan    |
| 01     |  5-9     | Blue Cyan |
| 02     | 10-14    | Blue      |
| 03     | 15-19    | Blue Magenta |
| 04     | 20-24    | Magenta   |
| 05     | 25-29    | Red Magenta  |
| 06     | 30-34    | Red       |
| 07     | 35-39    | Orange    |
| 08     | 40-44    | Yellow    |
| 09     | 45-49    | Green Yellow |
| 10     | 50-54    | Green     |
| 11     | 55-59    | Green Cyan   |


Hardware
----------------------------
The code uses and Arduino board with an ESP 8266 WiFi Module. 
Specifically, I used an Adafruit Feather HUZZAH with ESP8266
https://www.adafruit.com/product/2821

I used the WiFi network to periodically call NTP for current time/date and adjust the internal time accordingly. 
The idea is that this should be able to run and rather than get a clock chip ( I had the feather board on hand ), 
I just sync with an NTP server. The feather board also has a battery connection in case of power failure, and 
charges if power is being supplied to the usb connector.

The lights are a simple rgb light strip. The ones I used were from Adafruit:  
https://www.adafruit.com/product/1376


IDE and Libraries
------------------------------
I used Platform.io on Visual Studio Code to compile/load this. But should be fine for Arduino Studio. You may need to 
rename main.cpp -> main.ino for it to compile correctly. It also uses a few Arduino Libraries specified below in case
your IDE does not support loading/searching of libraries.

Adafruit NeoPixel  
https://github.com/adafruit/Adafruit_NeoPixel

ESP8266WiFi  
WiFiUdp  
https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

Timezone  
https://github.com/JChristensen/Timezone

Time  
https://github.com/PaulStoffregen/Time





