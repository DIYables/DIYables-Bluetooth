#include "DIYables_BluetoothPinControl.h"

DIYables_BluetoothPinControl::DIYables_BluetoothPinControl() 
    : DIYables_BluetoothAppBase(),
      pinWriteCallback(nullptr),
      pinReadCallback(nullptr),
      pinModeCallback(nullptr) {
    // Initialize all pins as disabled
    for (int i = 0; i < 16; i++) {
        enabledPins[i] = false;
        pinModes[i] = BT_PIN_OUTPUT;
        pinNames[i] = "Pin " + String(i);
    }
}

void DIYables_BluetoothPinControl::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Handle different message types
    if (msg == "PINS:GET_CONFIG") {
        DIYables_BluetoothAppBase::send(buildPinConfigJson());
    } else if (msg.startsWith("PINS:")) {
        handlePinCommand(msg);
    } else if (msg.startsWith("MODE:")) {
        handlePinModeCommand(msg);
    } else if (msg.startsWith("READ:")) {
        handleReadCommand(msg);
    } else if (msg == "PINS:GET_STATES") {
        DIYables_BluetoothAppBase::send(buildPinStatesJson());
    }
}

void DIYables_BluetoothPinControl::handlePinCommand(const String& message) {
    // Parse "PINS:pin,state" format (e.g., "PINS:13,1")
    int colonPos = message.indexOf(':');
    int commaPos = message.indexOf(',');
    
    if (colonPos != -1 && commaPos != -1) {
        int pin = message.substring(colonPos + 1, commaPos).toInt();
        int state = message.substring(commaPos + 1).toInt();
        
        if (pin >= 0 && pin < 16 && enabledPins[pin]) {
            if (pinWriteCallback) {
                pinWriteCallback(pin, state);
            }
            // Send confirmation back to app
            updatePinState(pin, state);
        }
    }
}

void DIYables_BluetoothPinControl::handlePinModeCommand(const String& message) {
    // Parse "MODE:pin,mode" format
    int colonPos = message.indexOf(':');
    int commaPos = message.indexOf(',');
    
    if (colonPos != -1 && commaPos != -1) {
        int pin = message.substring(colonPos + 1, commaPos).toInt();
        int mode = message.substring(commaPos + 1).toInt();
        
        if (pin >= 0 && pin < 16 && enabledPins[pin]) {
            pinModes[pin] = mode;
            if (pinModeCallback) {
                pinModeCallback(pin, mode);
            }
        }
    }
}

void DIYables_BluetoothPinControl::handleReadCommand(const String& message) {
    // Parse "READ:pin" format
    int colonPos = message.indexOf(':');
    
    if (colonPos != -1) {
        int pin = message.substring(colonPos + 1).toInt();
        
        if (pin >= 0 && pin < 16 && enabledPins[pin] && pinReadCallback) {
            int state = pinReadCallback(pin);
            updatePinState(pin, state);
        }
    }
}

String DIYables_BluetoothPinControl::buildPinConfigJson() {
    // CSV format: pin,name,type,value;pin,name,type,value;...
    String csv = "PINS_CONFIG:";
    bool first = true;
    const int MAX_MESSAGE_SIZE = 200; // Safe limit for Bluetooth transmission
    
    for (int i = 0; i < 16; i++) {
        if (enabledPins[i]) {
            if (!first) csv += ";";
            
            // Get current value if read callback exists
            int value = 0;
            if (pinReadCallback) {
                value = pinReadCallback(i);
            }
            
            csv += String(i) + ",";
            csv += pinNames[i] + ",";
            csv += String(pinModes[i] == BT_PIN_OUTPUT ? "o" : "i") + ",";
            csv += String(value);
            first = false;
            
            // Check message size and warn if approaching limit
            if (csv.length() > MAX_MESSAGE_SIZE) {
                Serial.println("WARNING: Pin config message too large!");
                Serial.print("Message size: ");
                Serial.print(csv.length());
                Serial.println(" bytes");
                Serial.println("Reduce number of pins or use shorter pin names.");
                Serial.println("Message will be truncated by Bluetooth!");
                break; // Stop adding more pins
            }
        }
    }
    
    if (csv.length() > MAX_MESSAGE_SIZE) {
        Serial.println("Final message size exceeds safe limit!");
    }
    
    return csv;
}

String DIYables_BluetoothPinControl::buildPinStatesJson() {
    String json = "PIN_STATES:{\"states\":[";
    bool first = true;
    
    for (int i = 0; i < 16; i++) {
        if (enabledPins[i] && pinReadCallback) {
            if (!first) json += ",";
            int state = pinReadCallback(i);
            json += "{\"pin\":" + String(i);
            json += ",\"state\":" + String(state) + "}";
            first = false;
        }
    }
    
    json += "]}";
    return json;
}

void DIYables_BluetoothPinControl::enablePin(int pin, int mode, const String& name) {
    if (pin >= 0 && pin < 16) {
        enabledPins[pin] = true;
        pinModes[pin] = mode;
        if (name.length() > 0) {
            pinNames[pin] = name;
        } else {
            pinNames[pin] = "Pin " + String(pin);
        }
    }
}

void DIYables_BluetoothPinControl::disablePin(int pin) {
    if (pin >= 0 && pin < 16) {
        enabledPins[pin] = false;
    }
}

void DIYables_BluetoothPinControl::enableAllPins(int mode) {
    for (int i = 0; i < 16; i++) {
        enabledPins[i] = true;
        pinModes[i] = mode;
    }
}

void DIYables_BluetoothPinControl::disableAllPins() {
    for (int i = 0; i < 16; i++) {
        enabledPins[i] = false;
    }
}

bool DIYables_BluetoothPinControl::isPinEnabled(int pin) {
    if (pin >= 0 && pin < 16) {
        return enabledPins[pin];
    }
    return false;
}

int DIYables_BluetoothPinControl::getPinMode(int pin) {
    if (pin >= 0 && pin < 16) {
        return pinModes[pin];
    }
    return BT_PIN_OUTPUT;
}

void DIYables_BluetoothPinControl::setPinMode(int pin, int mode) {
    if (pin >= 0 && pin < 16 && enabledPins[pin]) {
        pinModes[pin] = mode;
    }
}

int DIYables_BluetoothPinControl::getEnabledPinCount() {
    int count = 0;
    for (int i = 0; i < 16; i++) {
        if (enabledPins[i]) count++;
    }
    return count;
}

void DIYables_BluetoothPinControl::onPinWrite(PinWriteCallback callback) {
    pinWriteCallback = callback;
}

void DIYables_BluetoothPinControl::onPinRead(PinReadCallback callback) {
    pinReadCallback = callback;
}

void DIYables_BluetoothPinControl::onPinModeChange(PinModeCallback callback) {
    pinModeCallback = callback;
}

void DIYables_BluetoothPinControl::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPinControl::updatePinState(int pin, int state) {
    String message = "PINS:";
    message += String(pin);
    message += ",";
    message += String(state);
    DIYables_BluetoothAppBase::send(message);
}
