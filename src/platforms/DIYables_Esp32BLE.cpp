#if defined(ESP32)

#include "DIYables_Esp32BLE.h"

// Static instance pointer
DIYables_Esp32BLE* DIYables_Esp32BLE::instance = nullptr;

// --- Server Callbacks ---
void DIYables_Esp32BLE::ServerCallbacks::onConnect(BLEServer* pServer) {
    if (instance) {
        instance->deviceConnected = true;
    }
}

void DIYables_Esp32BLE::ServerCallbacks::onDisconnect(BLEServer* pServer) {
    if (instance) {
        instance->deviceConnected = false;
    }
}

// --- RX Characteristic Callbacks ---
void DIYables_Esp32BLE::RxCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    if (instance && instance->messageCallback) {
        String value = pCharacteristic->getValue().c_str();
        if (value.length() > 0) {
            instance->messageCallback(value.c_str(), value.length());
        }
    }
}

// --- Constructor ---
DIYables_Esp32BLE::DIYables_Esp32BLE(const char* deviceName,
                                     const char* serviceUUID,
                                     const char* txUUID,
                                     const char* rxUUID)
    : deviceName(deviceName),
      serviceUUID(serviceUUID),
      txUUID(txUUID),
      rxUUID(rxUUID),
      pServer(nullptr),
      pTxCharacteristic(nullptr),
      pRxCharacteristic(nullptr),
      deviceConnected(false),
      oldDeviceConnected(false),
      connectedCallback(nullptr),
      disconnectedCallback(nullptr),
      messageCallback(nullptr) {
    instance = this;
}

// --- IBluetooth Implementation ---

bool DIYables_Esp32BLE::begin() {
    BLEDevice::init(deviceName);

    // Create BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create BLE Service
    BLEService* pService = pServer->createService(serviceUUID);

    // Create TX Characteristic (notify)
    pTxCharacteristic = pService->createCharacteristic(
        txUUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

    // Create RX Characteristic (write)
    pRxCharacteristic = pService->createCharacteristic(
        rxUUID,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    pRxCharacteristic->setCallbacks(new RxCallbacks());

    // Start service
    pService->start();

    // Configure advertising with explicit control over packet contents.
    // BLE advertising packet is limited to 31 bytes. A 128-bit service UUID
    // takes 18 bytes, which leaves too little room for the device name.
    // Solution: Put service UUID in advertising data, device name in scan response.
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();

    // Advertising data: service UUID + flags (no name to save space)
    BLEAdvertisementData advData;
    advData.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);
    advData.setCompleteServices(BLEUUID(serviceUUID));
    pAdvertising->setAdvertisementData(advData);

    // Scan response data: device name (sent when phone requests scan response)
    BLEAdvertisementData scanResponseData;
    scanResponseData.setName(deviceName);
    pAdvertising->setScanResponseData(scanResponseData);

    BLEDevice::startAdvertising();
    Serial.println("BLE advertising started, device name: " + String(deviceName));

    return true;
}

bool DIYables_Esp32BLE::isConnected() {
    return deviceConnected;
}

void DIYables_Esp32BLE::end() {
    BLEDevice::deinit(false);
    deviceConnected = false;
    oldDeviceConnected = false;
}

void DIYables_Esp32BLE::close() {
    if (pServer && deviceConnected) {
        pServer->disconnect(pServer->getConnId());
    }
    deviceConnected = false;
}

bool DIYables_Esp32BLE::send(const char* message, uint16_t length) {
    if (!deviceConnected || !pTxCharacteristic) {
        return false;
    }

    pTxCharacteristic->setValue((uint8_t*)message, length);
    pTxCharacteristic->notify();
    return true;
}

void DIYables_Esp32BLE::loop() {
    // Handle connection state changes (edge detection)
    if (deviceConnected && !oldDeviceConnected) {
        // Just connected
        oldDeviceConnected = true;
        if (connectedCallback) {
            connectedCallback();
        }
    }

    if (!deviceConnected && oldDeviceConnected) {
        // Just disconnected
        oldDeviceConnected = false;
        if (disconnectedCallback) {
            disconnectedCallback();
        }
        // Restart advertising after disconnect
        delay(500);
        BLEDevice::startAdvertising();
    }
}

// --- Event Callback Setters ---
void DIYables_Esp32BLE::setOnConnected(ConnectionCallback callback) {
    connectedCallback = callback;
}

void DIYables_Esp32BLE::setOnDisconnected(ConnectionCallback callback) {
    disconnectedCallback = callback;
}

void DIYables_Esp32BLE::setOnMessage(MessageCallback callback) {
    messageCallback = callback;
}

#endif // defined(ESP32)
