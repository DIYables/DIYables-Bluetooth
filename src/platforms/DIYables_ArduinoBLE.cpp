#if !defined(ESP32)

#include "DIYables_ArduinoBLE.h"

// Static instance pointer
DIYables_ArduinoBLE* DIYables_ArduinoBLE::instance = nullptr;

DIYables_ArduinoBLE::DIYables_ArduinoBLE(const char* deviceName,
                    const char* serviceUUID,
                    const char* txUUID,
                    const char* rxUUID)
    : diyablesService(serviceUUID),
      txCharacteristic(txUUID, BLERead | BLENotify, 512),
      rxCharacteristic(rxUUID, BLEWrite | BLEWriteWithoutResponse, 512),
      deviceName(deviceName),
      connectedCallback(nullptr),
      disconnectedCallback(nullptr),
      messageCallback(nullptr) {
    instance = this;
}

bool DIYables_ArduinoBLE::begin() {
    // Initialize BLE
    if (!BLE.begin()) {
        return false;
    }
    
    // Set up BLE service
    BLE.setLocalName(deviceName);
    BLE.setAdvertisedService(diyablesService);
    
    // Add characteristics to service
    diyablesService.addCharacteristic(txCharacteristic);
    diyablesService.addCharacteristic(rxCharacteristic);
    
    // Add service
    BLE.addService(diyablesService);
    
    // Set initial values
    txCharacteristic.writeValue("");
    
    // Set event handlers
    BLE.setEventHandler(BLEConnected, bleConnectHandler);
    BLE.setEventHandler(BLEDisconnected, bleDisconnectHandler);
    rxCharacteristic.setEventHandler(BLEWritten, rxCharacteristicWritten);
    
    // Start advertising
    BLE.advertise();
    
    return true;
}

void DIYables_ArduinoBLE::end() {
    BLE.end();
    deviceConnected = false;
}

bool DIYables_ArduinoBLE::isConnected() {
    return deviceConnected;
}

void DIYables_ArduinoBLE::close() {
    BLE.disconnect();
    deviceConnected = false;
}

bool DIYables_ArduinoBLE::send(const char* message, uint16_t length) {
    if (!deviceConnected) {
        return false;
    }
    
    // BLE has MTU limits, so send as string (ArduinoBLE handles this)
    String msg(message);
    if (length < msg.length()) {
        msg = msg.substring(0, length);
    }
    
    txCharacteristic.writeValue(msg);
    return true;
}

void DIYables_ArduinoBLE::loop() {
    BLE.poll();
}

// Static callback handlers
void DIYables_ArduinoBLE::bleConnectHandler(BLEDevice central) {
    if (instance) {
        instance->deviceConnected = true;
        if (instance->connectedCallback) {
            instance->connectedCallback();
        }
    }
}

void DIYables_ArduinoBLE::bleDisconnectHandler(BLEDevice central) {
    if (instance) {
        instance->deviceConnected = false;
        if (instance->disconnectedCallback) {
            instance->disconnectedCallback();
        }
    }
}

void DIYables_ArduinoBLE::rxCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
    if (instance && instance->messageCallback) {
        String value = instance->rxCharacteristic.value();
        instance->messageCallback(value.c_str(), value.length());
    }
}

// Event callback setters
void DIYables_ArduinoBLE::setOnConnected(ConnectionCallback callback) {
    connectedCallback = callback;
}

void DIYables_ArduinoBLE::setOnDisconnected(ConnectionCallback callback) {
    disconnectedCallback = callback;
}

void DIYables_ArduinoBLE::setOnMessage(MessageCallback callback) {
    messageCallback = callback;
}

#endif // !defined(ESP32)

