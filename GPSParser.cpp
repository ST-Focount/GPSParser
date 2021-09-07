#include "GPSParser.h"

GPSParser::GPSParser(SoftwareSerial *serial) {
    _serial = serial;
}

void GPSParser::init(long baud) {
    _serial->begin(baud);
    send_command(PMTK_SET_NMEA_UPDATE_1HZ);
    send_command(PMTK_SET_NMEA_OUTPUT_GLLONLY);
}

void GPSParser::send_command(const char *cmd) {
    _serial->println(cmd);
}

String realLat = "";
String realLong= "";

void GPSParser::parse_lat_long() {
    uint16_t size = 0;
    uint8_t counter = 0;
    uint8_t coord_index = 0;

    char full_line[100];

    bool debug = false;

    if (!debug) {
        while (_serial->available()) {
            char c = _serial->read();

            if (c != '\n' && size <= 100) {
                full_line[size] = c;
                size++;
            } else {
                break;
            }
        }
    }

    //Serial.println(full_line);

    if (strstr(full_line, "$GPGLL") == NULL || strstr(full_line, "$PMTK") != NULL || strlen(full_line) < 8) {
        return;
    }

    char lat_deg[2];
    char lat_min[8];
    char long_deg[4];
    char long_min[8];

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

                    if (coord_index == 10) {
                        break;
                    }
                }
            }
        }
    }

    float _long_min = atof(long_min);
    float _lat_min = atof(lat_min);

    int _lat_deg = atoi(lat_deg);
    int _long_deg = atoi(long_deg);

    _lat_min = _lat_min / 60;
    _long_min = _long_min / 60;

    String result1 = String(_lat_deg);

    char latmin[8];

    dtostrf(_lat_min,8, 6, latmin);

    for (int i = 1; i < 8; i++) {
        result1 += latmin[i];
    }

    if (!result1.equals("0.000000")) {
        latitude = result1;
        realLat = result1;
    } else {
        latitude = realLat;
    }

    String result2 = String(_long_deg);

    char longmin[8];

    dtostrf(_long_min,8, 6, longmin);

    for (int i = 1; i < 8; i++) {
        result2 += longmin[i];
    }

    if (!result2.equals("0.000000")) {
        longitude = result2;
        realLong = result2;
    } else {
        longitude = realLong;
    }
}
