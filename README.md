# GPSParser

Parsing and calculating (deg and min to real location) latitude and longitude values from NMEA (National Marine Electronics Association) GPLL message.

Usable with STM32/ARM boards, Raspberry Pi, Arduino and computers etc. (you should compile it for your CPU arch)

I needed this parser because of Adafruit Ultimate Breakout v3 GPS. (Adafruit GPS library is too big for my microcontroller. It was crashing because of memory problems)

Important: You should set "debug" variable to false before using it for real parsing.
