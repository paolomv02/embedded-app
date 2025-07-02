# CodeMinds Embedded - ModestIoT Framework

Este proyecto demuestra el uso del **ModestIoT Nano-framework** para crear dispositivos IoT con programación orientada a objetos, arquitectura basada en eventos y comandos (CQRS-inspired), y componentes reutilizables.

## 🏗️ Arquitectura del Framework

### ModestIoT Nano-framework (C++ Edition)

El framework ModestIoT proporciona una base sólida para el desarrollo de dispositivos IoT con los siguientes principios:

- **Orientado a Objetos**: Clases base para sensores, actuadores y dispositivos
- **Basado en Eventos**: Comunicación asíncrona entre componentes
- **Comandos (CQRS-inspired)**: Separación clara entre lectura (eventos) y escritura (comandos)
- **Composición**: Dispositivos complejos construidos a partir de componentes simples

### Componentes Base

```cpp
EventHandler    // Interfaz para manejo de eventos
CommandHandler  // Interfaz para manejo de comandos
Sensor         // Clase base para sensores (genera eventos)
Actuator       // Clase base para actuadores (recibe comandos)
Device         // Combina EventHandler + CommandHandler
```

### Componentes Implementados

#### Sensores
- **GpsSensor**: Manejo de datos GPS con TinyGPSPlus
- **RfidSensor**: Simulación de detección RFID
- **UltrasoundSensor**: Sensor de distancia ultrasónico
- **Button**: Botón con detección de eventos

#### Actuadores
- **Led**: Control de LEDs con comandos (on/off/toggle)
- **RelayModule**: Control de módulos relay

#### Dispositivos Complejos
- **TrackingDevice**: Dispositivo completo con GPS, RFID y comunicación
- **CiaSteelFaucet**: Grifo inteligente con sensor de proximidad

## 🔧 Configuración del Proyecto

### Hardware Virtual (Wokwi)

El proyecto incluye chips personalizados para simulación:

- `gps-neo6m.chip.c/json`: Simula GPS NEO-6M con datos NMEA
- `rfid.chip.c/json`: Simula módulo RFID con múltiples UIDs

### Pins Utilizados

```cpp
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17
#define RFID_PIN 21
#define STATUS_LED_PIN 2
```

### Configuración de Red

```cpp
#define WIFI_SSID "Wokwi-GUEST"
#define DEVICE_ID "HC2956"
#define TRACKING_ENDPOINT "http://host.wokwi.internal:5000/api/v1/tracking"
#define RFID_ENDPOINT "http://host.wokwi.internal:5000/api/v1/sensor-scans/create"
```

## 📝 Uso del Framework

### Ejemplo Básico (sketch.ino)

```cpp
#include "chips/ModestIoT.h"

TrackingDevice* trackingDevice;

void setup() {
    trackingDevice = new TrackingDevice(
        GPS_RX_PIN, GPS_TX_PIN,     // GPS pins
        RFID_PIN,                   // RFID pin
        STATUS_LED_PIN,             // Status LED pin
        WIFI_SSID, WIFI_PASSWORD,   // WiFi credentials
        TRACKING_ENDPOINT,          // Server endpoints
        RFID_ENDPOINT,
        DEVICE_ID                   // Device identifier
    );
    
    trackingDevice->initialize();
}

void loop() {
    trackingDevice->update();
    delay(100);
}
```

### Ejemplo Avanzado (advanced_example.ino)

Demuestra:
- Manejo de eventos personalizados
- Envío de comandos específicos
- Composición de múltiples dispositivos
- Control manual y automático

## 🎯 Eventos y Comandos

### Eventos Principales

```cpp
GPS_DATA_EVENT          // Nuevos datos GPS disponibles
RFID_DETECTED_EVENT     // Tarjeta RFID detectada
BUTTON_PRESSED_EVENT    // Botón presionado
DISTANCE_MEASURED_EVENT // Nueva medición de distancia
```

### Comandos Principales

```cpp
// LED Control
TOGGLE_LED_COMMAND
TURN_ON_COMMAND
TURN_OFF_COMMAND

// Relay Control
ACTIVATE_RELAY_COMMAND
DEACTIVATE_RELAY_COMMAND

// Communication
SEND_GPS_DATA_COMMAND
SEND_RFID_DATA_COMMAND
CONNECT_WIFI_COMMAND

// Smart Faucet
START_WATER_FLOW_COMMAND
STOP_WATER_FLOW_COMMAND
```

## 🔄 Flujo de Datos

1. **Sensores** leen datos y generan **eventos**
2. **Dispositivos** reciben eventos y procesan datos
3. **CommunicationHandler** envía datos al servidor
4. **Comandos** permiten control externo de actuadores
5. **LEDs** proporcionan retroalimentación visual

## 🌐 Comunicación de Red

### Transmisión GPS
```json
{
    "id": 1,
    "device_id": "HC2956",
    "created_at": "2025-03-22T10:30:00Z",
    "latitude": -23.466417,
    "longitude": -51.840683
}
```

### Transmisión RFID
```json
{
    "rfidCode": "XX01X",
    "scanType": "ENTRY"
}
```

## 🔧 Extensibilidad

### Crear un Nuevo Sensor

```cpp
class MySensor : public Sensor {
public:
    static const int MY_EVENT_ID = 50;
    static const Event MY_EVENT;
    
    MySensor(int pin, EventHandler* handler = nullptr);
    void update(); // Implementar lógica de lectura
};
```

### Crear un Nuevo Actuador

```cpp
class MyActuator : public Actuator {
public:
    static const int MY_COMMAND_ID = 60;
    static const Command MY_COMMAND;
    
    MyActuator(int pin, CommandHandler* handler = nullptr);
    void handle(Command command) override;
};
```

### Crear un Dispositivo Complejo

```cpp
class MyDevice : public Device {
private:
    MySensor* sensor;
    MyActuator* actuator;
    
public:
    void on(Event event) override;
    void handle(Command command) override;
    void update();
};
```

## 📚 Beneficios del Framework

1. **Separación de Responsabilidades**: Cada clase tiene un propósito específico
2. **Reutilización**: Componentes pueden combinarse en diferentes configuraciones
3. **Testabilidad**: Interfaces claras facilitan pruebas unitarias
4. **Mantenibilidad**: Código organizado y documentado
5. **Escalabilidad**: Fácil agregar nuevos sensores y actuadores
6. **Flexibilidad**: Arquitectura basada en eventos permite respuestas dinámicas

## 🎖️ Licencia

Este proyecto utiliza el **ModestIoT Nano-framework (C++ Edition)** desarrollado por Angel Velasquez, licenciado bajo Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0).

## 👥 Contribución

Este proyecto es parte del curso de IoT - Ciclo 2025-01, desarrollado por el equipo **CodeMinds** para demostrar las mejores prácticas en desarrollo de firmware IoT con arquitecturas modernas y orientadas a objetos.