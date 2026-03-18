#include "DIYables_BluetoothJoystick.h"

DIYables_BluetoothJoystick::DIYables_BluetoothJoystick(bool autoReturn, float sensitivity) 
    : DIYables_BluetoothAppBase(), 
      joystickCallback(nullptr), 
      configCallback(nullptr),
      autoReturn(autoReturn), 
      sensitivity(sensitivity) {
}

void DIYables_BluetoothJoystick::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for joystick (with JOYSTICK: prefix)
    if (!msg.startsWith("JOYSTICK:")) {
        return;
    }
    
    // Handle request for joystick configuration
    if (msg == "JOYSTICK:GET_CONFIG") {
        // Always send config response
        String response = "JOYSTICK_CONFIG:{\"autoReturn\":";
        response += autoReturn ? "true" : "false";
        response += ",\"sensitivity\":";
        response += String(sensitivity, 1);
        response += "}";
        DIYables_BluetoothAppBase::send(response);
        
        // Optionally call user callback for additional actions
        if (configCallback) {
            configCallback();
        }
        return;
    }
    
    // Parse joystick coordinate commands like "JOYSTICK:50,-75"
    if (msg.length() > 9) {
        String values = msg.substring(9);  // Skip "JOYSTICK:" prefix
        
        int commaIndex = values.indexOf(',');
        if (commaIndex != -1) {
            int x = values.substring(0, commaIndex).toInt();
            int y = values.substring(commaIndex + 1).toInt();
            
            if (joystickCallback) {
                joystickCallback(x, y);
            }
        }
    }
}

void DIYables_BluetoothJoystick::setAutoReturn(bool enabled) {
    autoReturn = enabled;
}

bool DIYables_BluetoothJoystick::getAutoReturn() const {
    return autoReturn;
}

void DIYables_BluetoothJoystick::setSensitivity(float value) {
    sensitivity = value;
}

float DIYables_BluetoothJoystick::getSensitivity() const {
    return sensitivity;
}

void DIYables_BluetoothJoystick::onJoystickValue(JoystickCallback callback) {
    joystickCallback = callback;
}

void DIYables_BluetoothJoystick::onGetConfig(JoystickConfigCallback callback) {
    configCallback = callback;
}

void DIYables_BluetoothJoystick::send(const String& message) {
    // Use base class send method
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothJoystick::send(int x, int y) {
    String message = "JOYSTICK:";
    message += String(x);
    message += ",";
    message += String(y);
    DIYables_BluetoothAppBase::send(message);
}
