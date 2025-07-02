/**
 * @file TrackingDevice.cpp
 * @brief Implements the TrackingDevice class.
 *
 * A complete IoT device implementation that demonstrates the integration of multiple
 * sensors and communication capabilities using the Modest IoT Nano-framework.
 * Handles GPS tracking, RFID scanning, and data transmission.
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

#include "TrackingDevice.h"
#include <Arduino.h>

TrackingDevice::TrackingDevice(int gpsRxPin, int gpsTxPin, int rfidPin, int ledPin,
                               const String &wifiSSID, const String &wifiPassword,
                               const String &trackingUrl, const String &rfidUrl,
                               const String &deviceId)
    : lastUpdate(0), updateInterval(1000)
{

    // Initialize components with this device as event/command handler
    gpsSensor = new GpsSensor(gpsRxPin, gpsTxPin, 10000, this);
    rfidSensor = new RfidSensor(rfidPin, 5000, this);
    commHandler = new CommunicationHandler(wifiSSID, wifiPassword, trackingUrl, rfidUrl, deviceId);
    statusLed = new Led(ledPin, false, this);
}

void TrackingDevice::on(Event event)
{
    if (event == GpsSensor::GPS_DATA_EVENT)
    {
        Serial.println("GPS data event received");

        // Get GPS data and send it
        GpsData gpsData = gpsSensor->getLastData();
        if (gpsData.isValid && commHandler->isWiFiConnected())
        {
            commHandler->sendGpsData(gpsData);

            // Blink LED to indicate GPS data sent
            statusLed->handle(Led::TOGGLE_LED_COMMAND);
            delay(100);
            statusLed->handle(Led::TOGGLE_LED_COMMAND);
        }
    }
    else if (event == RfidSensor::RFID_DETECTED_EVENT)
    {
        Serial.println("RFID detection event received");

        // Get RFID data and send it
        RfidData rfidData = rfidSensor->getLastDetection();
        if (rfidData.isValid && commHandler->isWiFiConnected())
        {
            commHandler->sendRfidData(rfidData);

            // Quick double blink to indicate RFID scan sent
            for (int i = 0; i < 2; i++)
            {
                statusLed->handle(Led::TURN_ON_COMMAND);
                delay(150);
                statusLed->handle(Led::TURN_OFF_COMMAND);
                delay(150);
            }
        }
    }
}

void TrackingDevice::handle(Command command)
{
    // Forward commands to appropriate handlers
    if (command == CommunicationHandler::CONNECT_WIFI_COMMAND)
    {
        commHandler->handle(command);
    }
    else if (command == Led::TOGGLE_LED_COMMAND ||
             command == Led::TURN_ON_COMMAND ||
             command == Led::TURN_OFF_COMMAND)
    {
        statusLed->handle(command);
    }
}

void TrackingDevice::initialize()
{
    Serial.println("Initializing Tracking Device...");

    // Add RFID codes for simulation
    rfidSensor->addRfidCode("XX01X");
    rfidSensor->addRfidCode("YY02Y");
    rfidSensor->addRfidCode("ZZ03Z");

    // Connect to WiFi
    handle(CommunicationHandler::CONNECT_WIFI_COMMAND);

    // Indicate initialization complete
    for (int i = 0; i < 3; i++)
    {
        statusLed->handle(Led::TURN_ON_COMMAND);
        delay(200);
        statusLed->handle(Led::TURN_OFF_COMMAND);
        delay(200);
    }

    Serial.println("Tracking Device initialized successfully!");
}

void TrackingDevice::update()
{
    if (millis() - lastUpdate >= updateInterval)
    {
        // Update sensors
        gpsSensor->update();
        rfidSensor->update();

        // Check communication status
        commHandler->checkConnection();

        lastUpdate = millis();
    }
}

GpsSensor *TrackingDevice::getGpsSensor() const
{
    return gpsSensor;
}

RfidSensor *TrackingDevice::getRfidSensor() const
{
    return rfidSensor;
}

CommunicationHandler *TrackingDevice::getCommunicationHandler() const
{
    return commHandler;
}

TrackingDevice::~TrackingDevice()
{
    delete gpsSensor;
    delete rfidSensor;
    delete commHandler;
    delete statusLed;
}
