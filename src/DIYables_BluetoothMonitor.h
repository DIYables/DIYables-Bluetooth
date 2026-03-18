#ifndef DIYABLES_BLUETOOTH_MONITOR_PAGE_H
#define DIYABLES_BLUETOOTH_MONITOR_PAGE_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Monitor handler - manages monitoring interface
 */
class DIYables_BluetoothMonitor : public DIYables_BluetoothAppBase {
private:
    void (*messageCallback)(const String& message);
    
public:
    DIYables_BluetoothMonitor();
    
    // Inherited virtual methods
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Callback setter
    void onMonitorMessage(void (*callback)(const String& message));
    
    // Send message (public wrapper)
    void send(const String& message);
};

#endif
