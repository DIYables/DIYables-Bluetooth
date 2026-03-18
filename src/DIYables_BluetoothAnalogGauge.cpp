#include "DIYables_BluetoothAnalogGauge.h"

DIYables_BluetoothAnalogGauge::DIYables_BluetoothAnalogGauge(float minValue, float maxValue, const String& unit) 
    : DIYables_BluetoothAppBase(),
      valueCallback(nullptr),
      minValue(minValue),
      maxValue(maxValue),
      unit(unit) {
}

void DIYables_BluetoothAnalogGauge::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for gauge (with GAUGE: prefix)
    if (!msg.startsWith("GAUGE:")) {
        return;
    }
    
    // Handle request for gauge value
    if (msg == "GAUGE:GET_VALUE") {
        if (valueCallback) {
            valueCallback();
        }
        return;
    }
    
    // Handle request for gauge configuration
    if (msg == "GAUGE:GET_CONFIG") {
        String response = "GAUGE_CONFIG:{\"min\":";
        response += String(minValue, 1);
        response += ",\"max\":";
        response += String(maxValue, 1);
        response += ",\"unit\":\"";
        response += unit;
        response += "\"}";
        DIYables_BluetoothAppBase::send(response);
        return;
    }
}

void DIYables_BluetoothAnalogGauge::setRange(float min, float max) {
    minValue = min;
    maxValue = max;
}

void DIYables_BluetoothAnalogGauge::setUnit(const String& newUnit) {
    unit = newUnit;
}

float DIYables_BluetoothAnalogGauge::getMin() const {
    return minValue;
}

float DIYables_BluetoothAnalogGauge::getMax() const {
    return maxValue;
}

String DIYables_BluetoothAnalogGauge::getUnit() const {
    return unit;
}

void DIYables_BluetoothAnalogGauge::onValueRequest(GaugeValueCallback callback) {
    valueCallback = callback;
}

void DIYables_BluetoothAnalogGauge::send(float value) {
    String message = "GAUGE:";
    message += String(value, 2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothAnalogGauge::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}
