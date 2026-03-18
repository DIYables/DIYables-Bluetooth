#if defined(ESP32)

#include "DIYables_Esp32Bluetooth.h"

DIYables_Esp32Bluetooth::DIYables_Esp32Bluetooth(const char* deviceName)
    : deviceName(deviceName),
      connected(false),
      connectedCallback(nullptr),
      disconnectedCallback(nullptr),
      messageCallback(nullptr) {
}

bool DIYables_Esp32Bluetooth::begin() {
    if (!serialBT.begin(deviceName)) {
        return false;
    }
    return true;
}

bool DIYables_Esp32Bluetooth::isConnected() {
    return connected;
}

void DIYables_Esp32Bluetooth::end() {
    serialBT.end();
    connected = false;
}

void DIYables_Esp32Bluetooth::close() {
    serialBT.disconnect();
    connected = false;
}

bool DIYables_Esp32Bluetooth::send(const char* message, uint16_t length) {
    if (!connected) return false;
    serialBT.write((const uint8_t*)message, length);
    serialBT.write('\n');
    return true;
}

void DIYables_Esp32Bluetooth::loop() {
    // Check connection status changes
    bool currentlyConnected = serialBT.hasClient();
    
    if (currentlyConnected && !connected) {
        connected = true;
        if (connectedCallback) {
            connectedCallback();
        }
    } else if (!currentlyConnected && connected) {
        connected = false;
        if (disconnectedCallback) {
            disconnectedCallback();
        }
    }
    
    // Read incoming data
    while (serialBT.available()) {
        char c = serialBT.read();
        if (c == '\n') {
            if (receiveBuffer.length() > 0) {
                if (messageCallback) {
                    messageCallback(receiveBuffer.c_str(), receiveBuffer.length());
                }
                receiveBuffer = "";
            }
        } else if (c != '\r') {
            receiveBuffer += c;
        }
    }
}

void DIYables_Esp32Bluetooth::setOnConnected(ConnectionCallback callback) {
    connectedCallback = callback;
}

void DIYables_Esp32Bluetooth::setOnDisconnected(ConnectionCallback callback) {
    disconnectedCallback = callback;
}

void DIYables_Esp32Bluetooth::setOnMessage(MessageCallback callback) {
    messageCallback = callback;
}

#endif // defined(ESP32)
