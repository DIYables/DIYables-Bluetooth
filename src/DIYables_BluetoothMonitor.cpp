#include "DIYables_BluetoothMonitor.h"

DIYables_BluetoothMonitor::DIYables_BluetoothMonitor() 
    : DIYables_BluetoothAppBase(), messageCallback(nullptr) {
}

void DIYables_BluetoothMonitor::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    if (messageCallback) {
        messageCallback(msg);
    }
}

void DIYables_BluetoothMonitor::onMonitorMessage(void (*callback)(const String& message)) {
    messageCallback = callback;
}

void DIYables_BluetoothMonitor::send(const String& message) {
    // Child class can modify message here before sending
    // For now, just pass through to base class
    DIYables_BluetoothAppBase::send(message);
}
