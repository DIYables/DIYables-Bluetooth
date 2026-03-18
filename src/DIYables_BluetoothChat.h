#ifndef DIYABLES_BLUETOOTH_CHAT_H
#define DIYABLES_BLUETOOTH_CHAT_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Chat app - manages chat/messaging interface
 * Message format: "CHAT:message content"
 */
class DIYables_BluetoothChat : public DIYables_BluetoothAppBase {
public:
    // Callback type definition
    typedef void (*ChatMessageCallback)(const String& message);

private:
    ChatMessageCallback messageCallback;

public:
    DIYables_BluetoothChat();
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Callback setter
    void onChatMessage(ChatMessageCallback callback);
    
    // Send methods
    void send(const String& message);
};

#endif
