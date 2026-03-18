/*
 * DIYables Bluetooth Library - ESP32 BLE Chat Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Chat feature:
 * - Two-way text messaging via Bluetooth
 * - Receive messages from mobile app
 * - Send messages to mobile app
 * 
 * Compatible Boards:
 * - ESP32-WROOM-32
 * - ESP32-DevKitC
 * - ESP32-WROVER
 * - ESP32-S3
 * - ESP32-C3
 * - Any ESP32 board supporting BLE
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
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Chat";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
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
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Chat Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add chat app to server
  bluetoothServer.addApp(&bluetoothChat);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothChat.send("Hello! ESP32 BLE is ready to chat.");
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
    
    // Custom command handling
    if (message.equalsIgnoreCase("ping")) {
      bluetoothChat.send("pong!");
    } else if (message.equalsIgnoreCase("status")) {
      bluetoothChat.send("ESP32 BLE is running normally");
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
  bluetoothServer.loop();
  
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
