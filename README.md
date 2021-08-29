# GPSParser

Parsing and calculating (deg and min to real location) latitude and longitude values from NMEA (National Marine Electronics Association) GPLL message.

Usable with STM32/ARM boards, Arduino (you should do some modifications for serial connection etc. but the parsing thing is same)

I needed this parser because of Adafruit Ultimate Breakout v3 GPS. (Adafruit GPS library is too big for my microcontroller. It was crashing because of memory problems)

Important: You should set "debug" variable to false before using it for real parsing.
