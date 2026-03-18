#ifndef DIYABLES_ESP32BLUETOOTH_H
#define DIYABLES_ESP32BLUETOOTH_H

#if defined(ESP32)

#include "../interfaces/IBluetooth.h"
#include "BluetoothSerial.h"

/**
 * ESP32 Classic Bluetooth (BluetoothSerial) implementation of IBluetooth.
 * Uses ESP32's built-in Bluetooth Classic SPP (Serial Port Profile).
 * 
 * IMPORTANT: In Arduino IDE, go to Tools -> Partition Scheme
 * and select "Huge APP (3MB No OTA/1MB SPIFFS)" or "No OTA (Large APP)"
 */
class DIYables_Esp32Bluetooth : public IBluetooth {
private:
    BluetoothSerial serialBT;
    const char* deviceName;
    bool connected;
    String receiveBuffer;
    
    ConnectionCallback connectedCallback;
    ConnectionCallback disconnectedCallback;
    MessageCallback messageCallback;

public:
    DIYables_Esp32Bluetooth(const char* deviceName);
    virtual ~DIYables_Esp32Bluetooth() = default;
    
    bool begin() override;
    bool isConnected() override;
    void end() override;
    void close() override;
    bool send(const char* message, uint16_t length) override;
    void loop() override;
    
    void setOnConnected(ConnectionCallback callback) override;
    void setOnDisconnected(ConnectionCallback callback) override;
    void setOnMessage(MessageCallback callback) override;
};

#endif // defined(ESP32)

#endif
