#include "DIYables_BluetoothRotator.h"

DIYables_BluetoothRotator::DIYables_BluetoothRotator(int mode) 
    : DIYables_BluetoothAppBase(),
      rotatorCallback(nullptr),
      configCallback(nullptr),
      rotatorMode(mode),
      minAngle(0),
      maxAngle(360) {
}

DIYables_BluetoothRotator::DIYables_BluetoothRotator(int mode, float minAng, float maxAng) 
    : DIYables_BluetoothAppBase(),
      rotatorCallback(nullptr),
      configCallback(nullptr),
      rotatorMode(mode),
      minAngle(minAng),
      maxAngle(maxAng) {
}

void DIYables_BluetoothRotator::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    if (!msg.startsWith("ROTATOR:")) {
        return;
    }
    
    if (msg == "ROTATOR:GET_CONFIG") {
        if (configCallback) {
            configCallback();
        } else {
            String response = "ROTATOR:CONFIG:";
            response += String(rotatorMode);
            response += ",";
            response += String(minAngle, 1);
            response += ",";
            response += String(maxAngle, 1);
            DIYables_BluetoothAppBase::send(response);
        }
        return;
    }
    
    if (msg.length() > 8) {
        String angleStr = msg.substring(8);
        float angle = angleStr.toFloat();
        
        if (rotatorCallback) {
            rotatorCallback(angle);
        }
    }
}

void DIYables_BluetoothRotator::setRotatorMode(int mode, float minAng, float maxAng) {
    rotatorMode = mode;
    minAngle = minAng;
    maxAngle = maxAng;
}

int DIYables_BluetoothRotator::getRotatorMode() const {
    return rotatorMode;
}

float DIYables_BluetoothRotator::getMinAngle() const {
    return minAngle;
}

float DIYables_BluetoothRotator::getMaxAngle() const {
    return maxAngle;
}

void DIYables_BluetoothRotator::onRotatorAngle(RotatorCallback callback) {
    rotatorCallback = callback;
}

void DIYables_BluetoothRotator::onGetConfig(RotatorConfigCallback callback) {
    configCallback = callback;
}

void DIYables_BluetoothRotator::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothRotator::send(float angle) {
    String message = "ROTATOR:";
    message += String(angle, 1);
    DIYables_BluetoothAppBase::send(message);
}
