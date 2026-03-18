#ifndef DIYABLES_ARDUINO_BLE_H
#define DIYABLES_ARDUINO_BLE_H

#if !defined(ESP32)

#include "../interfaces/IBluetooth.h"
#include <ArduinoBLE.h>

class DIYables_ArduinoBLE : public IBluetooth {
private:
    // BLE Service and Characteristics
    BLEService diyablesService;
    BLEStringCharacteristic txCharacteristic;
    BLEStringCharacteristic rxCharacteristic;

    const char* deviceName;
    bool deviceConnected = false;
    
    // Callback handlers
    ConnectionCallback connectedCallback;
    ConnectionCallback disconnectedCallback;
    MessageCallback messageCallback;
    
    // Static instance for callbacks
    static DIYables_ArduinoBLE* instance;
    
    // Static callback handlers
    static void bleConnectHandler(BLEDevice central);
    static void bleDisconnectHandler(BLEDevice central);
    static void rxCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
    
public:
    DIYables_ArduinoBLE(const char* deviceName,
                        const char* serviceUUID,
                        const char* txUUID,
                        const char* rxUUID);
    virtual ~DIYables_ArduinoBLE() = default;
    
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

#endif // !defined(ESP32)

#endif
