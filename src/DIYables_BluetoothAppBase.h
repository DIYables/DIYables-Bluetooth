#ifndef DIYABLES_BLUETOOTH_APP_BASE_H
#define DIYABLES_BLUETOOTH_APP_BASE_H

#include "interfaces/IBluetooth.h"

/**
 * Abstract base class for Bluetooth apps.
 * Each app type (Monitor, DigitalPins, etc.) inherits from this class.
 */
class DIYables_BluetoothAppBase {
public:
    DIYables_BluetoothAppBase();
    virtual ~DIYables_BluetoothAppBase() = default;
    
    
    virtual void handleBluetoothMessage(const char* message, uint16_t length) = 0;
    
    
    // Callback management (per-connection event handling)
   
    virtual void onConnected();
    virtual void onClosed();
    
protected:
    IBluetooth* bluetooth;  // Reference to bluetooth interface
    
    // Send message via bluetooth
    bool send(const String& message);
    
    // Check if bluetooth is connected
    bool isConnected();
    
private:
    // Allow server to set reference
    friend class DIYables_BluetoothServer;
    void setBluetooth(IBluetooth* bt) { bluetooth = bt; }
};

#endif
