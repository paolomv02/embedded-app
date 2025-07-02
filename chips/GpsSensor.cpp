/**
 * @file GpsSensor.cpp
 * @brief Implements the GpsSensor class.
 *
 * Manages GPS data reading and event generation in the Modest IoT Nano-framework.
 * Processes NMEA sentences and generates events when new valid location data is available.
 *
 * @author Angel Velasquez
 * @date March 22, 2025
 * @version 0.1
 */

/*
 * This file is part of the Modest IoT Nano-framework (C++ Edition).
 * Copyright (c) 2025 Angel Velasquez
 *
 * Licensed under the Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0).
 * You may use, copy, and distribute this software in its original, unmodified form, provided
 * you give appropriate credit to the original author (Angel Velasquez) and include this notice.
 * Modifications, adaptations, or derivative works are not permitted.
 *
 * Full license text: https://creativecommons.org/licenses/by-nd/4.0/legalcode
 */

#include "GpsSensor.h"
#include <Arduino.h>

const Event GpsSensor::GPS_DATA_EVENT = Event(GPS_DATA_EVENT_ID);

GpsSensor::GpsSensor(int rxPin, int txPin, unsigned long updateInterval, EventHandler *eventHandler)
    : Sensor(-1, eventHandler), updateInterval(updateInterval), lastUpdate(0)
{
    gpsSerial = &Serial2;
    gpsSerial->begin(9600, SERIAL_8N1, rxPin, txPin);
    lastValidData.isValid = false;
}

void GpsSensor::update()
{
    // Read available GPS data
    while (gpsSerial->available() > 0)
    {
        char c = gpsSerial->read();
        gps.encode(c);
    }

    // Check if we have new location data and enough time has passed
    if (gps.location.isUpdated() && (millis() - lastUpdate > updateInterval))
    {
        if (gps.location.isValid())
        {
            lastValidData.latitude = gps.location.lat();
            lastValidData.longitude = gps.location.lng();
            lastValidData.isValid = true;

            // Generate timestamp
            time_t now = time(nullptr);
            struct tm timeinfo;
            char buf[30];
            gmtime_r(&now, &timeinfo);
            strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
            lastValidData.timestamp = String(buf);

            lastUpdate = millis();

            // Trigger GPS data event
            on(GPS_DATA_EVENT);
        }
    }
}

GpsData GpsSensor::getLastData() const
{
    return lastValidData;
}

bool GpsSensor::hasValidFix() const
{
    return lastValidData.isValid && gps.location.isValid();
}
