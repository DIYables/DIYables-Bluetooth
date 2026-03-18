/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Chat Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Chat feature:
 * - Two-way text messaging via Bluetooth
 * - Receive messages from mobile app
 * - Send messages to mobile app
 * 
 * Compatible Boards:
 * - ESP32 (all variants with Classic Bluetooth)
 * - ESP32-WROOM-32
 * - ESP32-DevKitC
 * - ESP32-WROVER
 * 
 * Note: Select "Huge APP (3MB No OTA/1MB SPIFFS)" partition scheme
 *       in Arduino IDE: Tools > Partition Scheme
 * 
 * Setup:
 * 1. Upload the sketch to your ESP32
 * 2. Open Serial Monitor (115200 baud) to see connection status and messages
 * 3. Use DIYables Bluetooth App to connect and chat
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothChat.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Chat");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Chat app instance
DIYables_BluetoothChat bluetoothChat;

// Variables for periodic messages
unsigned long lastMessageTime = 0;
const unsigned long MESSAGE_INTERVAL = 10000;  // Send message every 10 seconds
int messageCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Chat Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add chat app to server
  bluetoothServer.addApp(&bluetoothChat);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothChat.send("Hello! ESP32 is ready to chat.");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
    messageCount = 0;
  });
  
  // Set up callback for received chat messages
  bluetoothChat.onChatMessage([](const String& message) {
    Serial.print("Received: ");
    Serial.println(message);
    
    // Echo the message back
    String response = "Echo: ";
    response += message;
    bluetoothChat.send(response);
    
    // You can add custom command handling here
    if (message.equalsIgnoreCase("ping")) {
      bluetoothChat.send("pong!");
    } else if (message.equalsIgnoreCase("status")) {
      bluetoothChat.send("ESP32 is running normally");
    } else if (message.equalsIgnoreCase("time")) {
      String timeMsg = "Uptime: ";
      timeMsg += String(millis() / 1000);
      timeMsg += " seconds";
      bluetoothChat.send(timeMsg);
    } else if (message.equalsIgnoreCase("heap")) {
      String heapMsg = "Free heap: ";
      heapMsg += String(ESP.getFreeHeap());
      heapMsg += " bytes";
      bluetoothChat.send(heapMsg);
    }
  });
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.println("Type 'ping', 'status', 'time', or 'heap' in the app to test commands");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send periodic status message (only when connected)
  if (bluetooth.isConnected() && millis() - lastMessageTime >= MESSAGE_INTERVAL) {
    lastMessageTime = millis();
    messageCount++;
    
    String statusMsg = "Status update #";
    statusMsg += String(messageCount);
    statusMsg += " - All systems operational";
    bluetoothChat.send(statusMsg);
    
    Serial.print("Sent: ");
    Serial.println(statusMsg);
  }
  
  // Optional: Read from Serial and send to Bluetooth
  if (Serial.available()) {
    String serialMsg = Serial.readStringUntil('\n');
    serialMsg.trim();
    if (serialMsg.length() > 0 && bluetooth.isConnected()) {
      bluetoothChat.send(serialMsg);
      Serial.print("Sent from Serial: ");
      Serial.println(serialMsg);
    }
  }
  
  delay(10);
}
