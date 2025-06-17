#include "GPSSensor.h"

const Event GPSSensor::GPS_COORDINATE_UPDATED_EVENT(GPS_COORDINATE_UPDATED_EVENT_ID);

GPSSensor::GPSSensor(HardwareSerial* serial, EventHandler *handler)
    : Sensor(-1, handler), latitude(0), longitude(0), gpsSerial(serial) {}

void GPSSensor::begin(uint32_t baud) {
    gpsSerial->begin(baud);
}

void GPSSensor::update() {
    bool updated = false;
    while (gpsSerial->available() > 0) {
        char c = gpsSerial->read();
        if (gps.encode(c)) {
            if (gps.location.isValid() && gps.location.isUpdated()) {
                double newLat = gps.location.lat();
                double newLng = gps.location.lng();
                if (newLat != latitude || newLng != longitude) {
                    latitude = newLat;
                    longitude = newLng;
                    updated = true;
                }
            }
        }
    }
    if (updated) {
        on(GPS_COORDINATE_UPDATED_EVENT);
    }
}

double GPSSensor::getLatitude() const {
    return latitude;
}

double GPSSensor::getLongitude() const {
    return longitude;
}