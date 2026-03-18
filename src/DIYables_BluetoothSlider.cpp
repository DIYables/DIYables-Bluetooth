#include "DIYables_BluetoothSlider.h"

DIYables_BluetoothSlider::DIYables_BluetoothSlider(int min, int max, int step) 
    : DIYables_BluetoothAppBase(),
      sliderCallback(nullptr),
      configCallback(nullptr),
      minValue(min),
      maxValue(max),
      stepValue(step) {
}

void DIYables_BluetoothSlider::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for slider (with SLIDER: prefix)
    if (!msg.startsWith("SLIDER:")) {
        return;
    }
    
    // Handle request for slider configuration
    if (msg == "SLIDER:GET_CONFIG") {
        // Always send config first
        String response = "SLIDER_CONFIG:{\"min\":";
        response += String(minValue);
        response += ",\"max\":";
        response += String(maxValue);
        response += ",\"step\":";
        response += String(stepValue);
        response += "}";
        DIYables_BluetoothAppBase::send(response);
        
        // Then call user callback if registered (e.g., to send current values)
        if (configCallback) {
            configCallback();
        }
        return;
    }
    
    // Parse slider value commands like "SLIDER:50,75"
    if (msg.length() > 7) {
        String values = msg.substring(7);  // Skip "SLIDER:" prefix
        
        int commaIndex = values.indexOf(',');
        if (commaIndex != -1) {
            // Two slider values
            int slider1 = values.substring(0, commaIndex).toInt();
            int slider2 = values.substring(commaIndex + 1).toInt();
            
            if (sliderCallback) {
                sliderCallback(slider1, slider2);
            }
        } else {
            // Single slider value
            int value = values.toInt();
            if (sliderCallback) {
                sliderCallback(value, value);  // Use same value for both
            }
        }
    }
}

void DIYables_BluetoothSlider::setRange(int min, int max) {
    minValue = min;
    maxValue = max;
}

void DIYables_BluetoothSlider::setStep(int step) {
    stepValue = step;
}

int DIYables_BluetoothSlider::getMin() const {
    return minValue;
}

int DIYables_BluetoothSlider::getMax() const {
    return maxValue;
}

int DIYables_BluetoothSlider::getStep() const {
    return stepValue;
}

void DIYables_BluetoothSlider::onSliderValue(SliderCallback callback) {
    sliderCallback = callback;
}

void DIYables_BluetoothSlider::onGetConfig(SliderConfigCallback callback) {
    configCallback = callback;
}

void DIYables_BluetoothSlider::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothSlider::send(int slider1, int slider2) {
    String message = "SLIDER:";
    message += String(slider1);
    message += ",";
    message += String(slider2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothSlider::send(int value) {
    String message = "SLIDER:";
    message += String(value);
    DIYables_BluetoothAppBase::send(message);
}
