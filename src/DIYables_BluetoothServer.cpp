#include "DIYables_BluetoothServer.h"

// Static member initialization
DIYables_BluetoothServer* DIYables_BluetoothServer::instance = nullptr;

DIYables_BluetoothServer::DIYables_BluetoothServer(IBluetooth& bluetooth)
    : bluetooth(&bluetooth), appCount(0), userConnectedCallback(nullptr), userDisconnectedCallback(nullptr) {
    instance = this;
    
    // Initialize apps array
    for (int i = 0; i < MAX_APPS; i++) {
        apps[i] = nullptr;
    }
}

DIYables_BluetoothServer::~DIYables_BluetoothServer() {
    if (instance == this) {
        instance = nullptr;
    }
}

bool DIYables_BluetoothServer::begin() {
    if (!bluetooth) {
        return false;
    }
    
    // Register callbacks with the Bluetooth implementation
    bluetooth->setOnConnected([]() {
        DIYables_BluetoothServer::onConnected();
    });
    
    bluetooth->setOnDisconnected([]() {
        DIYables_BluetoothServer::onClosed();
    });
    
    bluetooth->setOnMessage([](const char* msg, uint16_t len) {
        DIYables_BluetoothServer::onMessage(msg, len);
    });
    
    return bluetooth->begin();
}

void DIYables_BluetoothServer::loop() {
    if (bluetooth) {
        bluetooth->loop();
    }
}

bool DIYables_BluetoothServer::addApp(DIYables_BluetoothAppBase* app) {
    if (!app || appCount >= MAX_APPS) {
        return false;
    }
    
    apps[appCount++] = app;
    app->setBluetooth(bluetooth);
    return true;
}

bool DIYables_BluetoothServer::isConnected() {
    return bluetooth ? bluetooth->isConnected() : false;
}

bool DIYables_BluetoothServer::send(const String& message) {
    if (!bluetooth || !bluetooth->isConnected()) {
        return false;
    }
    return bluetooth->send(message);
}

// Static callbacks
void DIYables_BluetoothServer::onConnected() {
    if (instance) {
        // Call user callback if set
        if (instance->userConnectedCallback) {
            instance->userConnectedCallback();
        }
        
        // Notify all apps
        for (int i = 0; i < instance->appCount; i++) {
            if (instance->apps[i]) {
                instance->apps[i]->onConnected();
            }
        }
    }
}

void DIYables_BluetoothServer::onMessage(const char* message, uint16_t length) {
    if (instance) {
        for (int i = 0; i < instance->appCount; i++) {
            if (instance->apps[i]) {
                instance->apps[i]->handleBluetoothMessage(message, length);
            }
        }
    }
}

void DIYables_BluetoothServer::onClosed() {
    if (instance) {
        // Call user callback if set
        if (instance->userDisconnectedCallback) {
            instance->userDisconnectedCallback();
        }
        
        // Notify all apps
        for (int i = 0; i < instance->appCount; i++) {
            if (instance->apps[i]) {
                instance->apps[i]->onClosed();
            }
        }
    }
}
