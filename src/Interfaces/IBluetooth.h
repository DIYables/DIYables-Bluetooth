#ifndef I_BLUETOOTH_H
#define I_BLUETOOTH_H

#include <Arduino.h>

/**
 * Abstract interface for Bluetooth connections.
 * This abstracts platform-specific Bluetooth implementations (BLE, Classic, etc.).
 */
class IBluetooth {
public:
    // Callback types
    typedef void (*ConnectionCallback)();
    typedef void (*MessageCallback)(const char* message, uint16_t length);
    
    virtual ~IBluetooth() = default;
    
    // Connection management
    virtual bool begin() = 0;
    virtual bool isConnected() = 0;
    virtual void end() = 0;
    virtual void close() = 0;
    
    // Message handling
    virtual bool send(const char* message, uint16_t length) = 0;
    virtual bool send(const String& message) {
        return send(message.c_str(), message.length());
    }
    
    // Event callbacks registration
    virtual void setOnConnected(ConnectionCallback callback) = 0;
    virtual void setOnDisconnected(ConnectionCallback callback) = 0;
    virtual void setOnMessage(MessageCallback callback) = 0;
    
    // Loop processing
    virtual void loop() = 0;
};

#endif
