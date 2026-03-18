#ifndef DIYABLES_BLUETOOTH_TEMPERATURE_H
#define DIYABLES_BLUETOOTH_TEMPERATURE_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Temperature app - manages temperature sensor display
 * Message format: "TEMPERATURE:value" or "TEMPERATURE:GET_VALUE"
 */
class DIYables_BluetoothTemperature : public DIYables_BluetoothAppBase {
public:
    // Callback type definition
    typedef void (*TemperatureCallback)();

private:
    TemperatureCallback temperatureCallback;
    float minTemp;
    float maxTemp;
    String unit;

public:
    DIYables_BluetoothTemperature(float minTemp = 0.0, float maxTemp = 100.0, const String& unit = "°C");
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Configuration methods
    void setRange(float min, float max);
    void setUnit(const String& unit);
    float getMin() const;
    float getMax() const;
    String getUnit() const;
    
    // Callback setter
    void onTemperatureRequest(TemperatureCallback callback);
    
    // Send methods
    void send(float temperature);
    void send(const String& message);
};

#endif
