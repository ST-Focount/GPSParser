#include "GPSParser.h"

GPSParser::GPSParser(SoftwareSerial *serial) {
    _serial = serial;
}

void GPSParser::init(int baud) {
    _serial->begin(baud);
    send_command(PMTK_SET_NMEA_UPDATE_1HZ);
    send_command(PMTK_SET_NMEA_OUTPUT_GLLONLY);
}

void GPSParser::send_command(const char *cmd) {
    _serial->println(cmd);
}

void GPSParser::parse_lat_long() {
    uint8_t size = 0;
    uint8_t counter = 0;
    uint8_t coord_index = 0;

    char full_line[] = "$GPGLL,4044.2413,N,03020.1276,E,102218.22";

    bool debug = true;

    if (!debug) {
        while (_serial->available()) {
            char c = _serial->read();
            if (c != '\n') {
                full_line[size] = c;
                size++;
            }
        }
    }

    if (strstr(full_line, "$GPGLL") == NULL || strlen(full_line) < 8) {
        return;
    }

    char lat_deg[2];
    char lat_min[8];

    char long_deg[4];
    char long_min[8];

    char *lat_str;
    char *long_str;

    bool gpgll_checker = false;

    for (int i = 0; i < strlen(full_line); i++) {
        char c = full_line[i];

        if (!gpgll_checker && i > 4) {
            gpgll_checker = full_line[i + 1] != ',' && c == ',' && full_line[i - 1] == 'L' && full_line[i - 2] == 'L' && full_line[i - 3] == 'G';
        }

        if (gpgll_checker) {
            if (c == ',') {
                counter++;
                coord_index = 0;
            } else {
                if (counter == 1) {
                    if (coord_index <= 1) {
                        lat_deg[coord_index] = c;
                    } else if (coord_index > 1 && coord_index <= 8) {
                        lat_min[coord_index - 2] = c;
                    }

                    coord_index++;
                } else if (counter == 3) {
                    if (coord_index <= 2) {
                        long_deg[coord_index] = c;
                    } else if (coord_index >= 3 && coord_index <= 9) {
                        long_min[coord_index - 3] = c;
                    }

                    coord_index++;
                }
            }
        }
    }

    float _long_min = atof(long_min);
    float _lat_min = atof(lat_min);

    int _lat_deg = atoi(lat_deg) / 10;
    int _long_deg = atoi(long_deg) / 10;

    _lat_min = _lat_min / 60;
    _long_min = _long_min / 60;

    latitude = String(_lat_deg) + String(_lat_min, 4);
    longitude = String(_long_deg) + String(_long_min, 4);
}