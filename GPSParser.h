#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#define PMTK_SET_NMEA_OUTPUT_GLLONLY "$PMTK314,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29" 
#define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,250*29"

#include <SoftwareSerial.h>
#include "Arduino.h"

class GPSParser {
    SoftwareSerial *_serial;

    long latitude = 0;
    long longitude = 0;

    public:
        GPSParser(SoftwareSerial *serial);

    void init(int baud);
    void parse_lat_long();
    void send_command(const char* cmd);
};

#endif