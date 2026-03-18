#include "DIYables_BluetoothAppBase.h"

DIYables_BluetoothAppBase::DIYables_BluetoothAppBase() 
    : bluetooth(nullptr) {
}

void DIYables_BluetoothAppBase::onConnected() {
    // Default implementation - can be overridden
}

void DIYables_BluetoothAppBase::onClosed() {
    // Default implementation - can be overridden
}

bool DIYables_BluetoothAppBase::send(const String& message) {
    if (bluetooth && bluetooth->isConnected()) {
        return bluetooth->send(message);
    }
    return false;
}

bool DIYables_BluetoothAppBase::isConnected() {
    return bluetooth ? bluetooth->isConnected() : false;
}
