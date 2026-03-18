#include "DIYables_BluetoothTemperature.h"

DIYables_BluetoothTemperature::DIYables_BluetoothTemperature(float minTemp, float maxTemp, const String& unit) 
    : DIYables_BluetoothAppBase(),
      temperatureCallback(nullptr),
      minTemp(minTemp),
      maxTemp(maxTemp),
      unit(unit) {
}

void DIYables_BluetoothTemperature::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for temperature (with TEMPERATURE: prefix)
    if (!msg.startsWith("TEMPERATURE:")) {
        return;
    }
    
    // Handle request for temperature value
    if (msg == "TEMPERATURE:GET_VALUE") {
        if (temperatureCallback) {
            temperatureCallback();
        }
        return;
    }
    
    // Handle request for temperature configuration
    if (msg == "TEMPERATURE:GET_CONFIG") {
        String response = "TEMPERATURE_CONFIG:{\"min\":";
        response += String(minTemp, 1);
        response += ",\"max\":";
        response += String(maxTemp, 1);
        response += ",\"unit\":\"";
        response += unit;
        response += "\"}";
        DIYables_BluetoothAppBase::send(response);
        return;
    }
}

void DIYables_BluetoothTemperature::setRange(float min, float max) {
    minTemp = min;
    maxTemp = max;
}

void DIYables_BluetoothTemperature::setUnit(const String& newUnit) {
    unit = newUnit;
}

float DIYables_BluetoothTemperature::getMin() const {
    return minTemp;
}

float DIYables_BluetoothTemperature::getMax() const {
    return maxTemp;
}

String DIYables_BluetoothTemperature::getUnit() const {
    return unit;
}

void DIYables_BluetoothTemperature::onTemperatureRequest(TemperatureCallback callback) {
    temperatureCallback = callback;
}

void DIYables_BluetoothTemperature::send(float temperature) {
    String message = "TEMPERATURE:";
    message += String(temperature, 1);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothTemperature::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}
