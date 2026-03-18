#ifndef DIYABLES_BLUETOOTH_ANALOG_GAUGE_H
#define DIYABLES_BLUETOOTH_ANALOG_GAUGE_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Analog Gauge app - manages analog gauge display
 * Message format: "GAUGE:value" or "GAUGE:GET_VALUE"
 * Displays value on analog meter with configurable range and unit
 */
class DIYables_BluetoothAnalogGauge : public DIYables_BluetoothAppBase {
public:
    // Callback type definition
    typedef void (*GaugeValueCallback)();

private:
    GaugeValueCallback valueCallback;
    float minValue;
    float maxValue;
    String unit;

public:
    DIYables_BluetoothAnalogGauge(float minVal, float maxVal, const String& unit);
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Configuration methods
    void setRange(float min, float max);
    void setUnit(const String& unit);
    float getMin() const;
    float getMax() const;
    String getUnit() const;
    
    // Callback setter
    void onValueRequest(GaugeValueCallback callback);
    
    // Send methods
    void send(float value);
    void send(const String& message);
};

#endif
