#ifndef DIYABLES_ESP32BLE_H
#define DIYABLES_ESP32BLE_H

#if defined(ESP32)

#include "../interfaces/IBluetooth.h"

// IMPORTANT: We intentionally do NOT include <BLEDevice.h> here.
// Both ArduinoBLE and ESP32's native BLE library define BLEDevice.h,
// and the Arduino IDE resolves includes as text (ignoring #ifdef guards),
// which causes class redefinition errors when both libraries are present.
//
// Instead, we include <BLEServer.h> and <BLE2902.h> which are UNIQUE
// to ESP32's native BLE library. BLEServer.h internally includes
// BLEDevice.h from its own directory using a relative #include "BLEDevice.h",
// which the compiler correctly resolves to ESP32's version.
#include <BLEServer.h>
#include <BLE2902.h>

/**
 * ESP32 BLE (Bluetooth Low Energy) implementation of IBluetooth.
 * Uses ESP32's built-in BLE stack (BLEDevice, BLEServer, BLECharacteristic).
 * 
 * IMPORTANT: In Arduino IDE, go to Tools -> Partition Scheme
 * and select "Huge APP (3MB No OTA/1MB SPIFFS)" or "No OTA (Large APP)"
 */
class DIYables_Esp32BLE : public IBluetooth {
private:
    const char* deviceName;
    const char* serviceUUID;
    const char* txUUID;
    const char* rxUUID;

    BLEServer* pServer;
    BLECharacteristic* pTxCharacteristic;
    BLECharacteristic* pRxCharacteristic;

    bool deviceConnected;
    bool oldDeviceConnected;

    ConnectionCallback connectedCallback;
    ConnectionCallback disconnectedCallback;
    MessageCallback messageCallback;

    // Static instance for callbacks
    static DIYables_Esp32BLE* instance;

    // Inner class for BLE server connection callbacks
    class ServerCallbacks : public BLEServerCallbacks {
        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
    };

    // Inner class for RX characteristic write callbacks
    class RxCallbacks : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic* pCharacteristic) override;
    };

public:
    DIYables_Esp32BLE(const char* deviceName,
                      const char* serviceUUID,
                      const char* txUUID,
                      const char* rxUUID);
    virtual ~DIYables_Esp32BLE() = default;

    // IBluetooth interface implementation
    bool begin() override;
    bool isConnected() override;
    void end() override;
    void close() override;
    bool send(const char* message, uint16_t length) override;
    void loop() override;

    // Event callback setters
    void setOnConnected(ConnectionCallback callback) override;
    void setOnDisconnected(ConnectionCallback callback) override;
    void setOnMessage(MessageCallback callback) override;
};

#endif // defined(ESP32)

#endif
