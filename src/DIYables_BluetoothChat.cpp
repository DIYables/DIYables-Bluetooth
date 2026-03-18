#include "DIYables_BluetoothChat.h"

DIYables_BluetoothChat::DIYables_BluetoothChat() 
    : DIYables_BluetoothAppBase(),
      messageCallback(nullptr) {
}

void DIYables_BluetoothChat::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for chat (with CHAT: prefix)
    if (!msg.startsWith("CHAT:")) {
        return;
    }
    
    // Extract message content after "CHAT:" prefix
    String chatMessage = msg.substring(5);
    
    if (messageCallback) {
        messageCallback(chatMessage);
    }
}

void DIYables_BluetoothChat::onChatMessage(ChatMessageCallback callback) {
    messageCallback = callback;
}

void DIYables_BluetoothChat::send(const String& message) {
    String formattedMessage = "CHAT:";
    formattedMessage += message;
    DIYables_BluetoothAppBase::send(formattedMessage);
}
