/*
 * DIYables Bluetooth Library - Bluetooth Chat Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Chat feature:
 * - Two-way text messaging via Bluetooth
 * - Receive messages from mobile app
 * - Send messages to mobile app
 * 
 * Compatible Boards:
 * - Arduino UNO R4 WiFi
 * - Arduino Nano 33 BLE / BLE Sense
 * - Arduino Nano 33 IoT
 * - Arduino MKR WiFi 1010
 * - Arduino Nano RP2040 Connect
 * - Any board supporting the ArduinoBLE library
 * 
 * Setup:
 * 1. Upload the sketch to your Arduino
 * 2. Open Serial Monitor to see connection status and messages
 * 3. Use DIYables Bluetooth App to connect and chat
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothChat.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Chat";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Chat app instance
DIYables_BluetoothChat bluetoothChat;

// Variables for periodic messages
unsigned long lastMessageTime = 0;
const unsigned long MESSAGE_INTERVAL = 10000;  // Send message every 10 seconds
int messageCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Chat Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add chat app to server
  bluetoothServer.addApp(&bluetoothChat);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothChat.send("Hello! Arduino is ready to chat.");
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
      bluetoothChat.send("Arduino is running normally");
    } else if (message.equalsIgnoreCase("time")) {
      String timeMsg = "Uptime: ";
      timeMsg += String(millis() / 1000);
      timeMsg += " seconds";
      bluetoothChat.send(timeMsg);
    }
  });
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.println("Type 'ping', 'status', or 'time' in the app to test commands");
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
