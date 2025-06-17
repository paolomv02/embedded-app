#ifndef GPSSENSOR_H
#define GPSSENSOR_H

#include "Sensor.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

/**
 * @file GpsSensor.h
 * @brief Declares the GpsSensor class for ModestIoT.
 *
 * This class represents a GPS sensor that reads coordinates and generates an event when updated.
 */

class GPSSensor : public Sensor
{
public:
    static const int GPS_COORDINATE_UPDATED_EVENT_ID = 100;
    static const Event GPS_COORDINATE_UPDATED_EVENT;

    GPSSensor(HardwareSerial* serial, EventHandler *handler = nullptr);

    void begin(uint32_t baud = 9600); // Inicializa el puerto serie del GPS
    void update(); // Call periodically to read GPS data
    double getLatitude() const;
    double getLongitude() const;

private:
    double latitude;
    double longitude;
    TinyGPSPlus gps;
    HardwareSerial* gpsSerial;
};

#endif