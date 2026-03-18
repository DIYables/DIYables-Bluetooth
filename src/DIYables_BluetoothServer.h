#ifndef DIYABLES_BLUETOOTH_SERVER_H
#define DIYABLES_BLUETOOTH_SERVER_H

#include "interfaces/IBluetooth.h"
#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth server class that manages Bluetooth connection
 * and routes messages to app handlers.
 */
class DIYables_BluetoothServer {
public:
    // Callback type for Bluetooth events
    typedef void (*BluetoothEventCallback)();
    
private:
    IBluetooth* bluetooth;
    
    // Dynamic app handlers storage
    static const int MAX_APPS = 10;
    DIYables_BluetoothAppBase* apps[MAX_APPS];
    int appCount;
    
    // User callbacks
    BluetoothEventCallback userConnectedCallback;
    BluetoothEventCallback userDisconnectedCallback;
    
    // Static callbacks for Bluetooth events (called from lambda in begin())
    static void onConnected();
    static void onMessage(const char* message, uint16_t length);
    static void onClosed();
    
public:
    // Static instance pointer for accessing from static callbacks
    static DIYables_BluetoothServer* instance;
    
    DIYables_BluetoothServer(IBluetooth& bluetooth);
    ~DIYables_BluetoothServer();
    
    // Basic setup methods
    bool begin();  
    void loop();
    
    // App management
    bool addApp(DIYables_BluetoothAppBase* app);
    int getAppCount() const { return appCount; }
    
    // Connection status
    bool isConnected();
    
    // Send message through bluetooth
    bool send(const String& message);
    
    // Set user callbacks for connection events
    void setOnConnected(BluetoothEventCallback callback) { userConnectedCallback = callback; }
    void setOnDisconnected(BluetoothEventCallback callback) { userDisconnectedCallback = callback; }
};

#endif
