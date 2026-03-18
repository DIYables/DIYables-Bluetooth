/*
 * DIYables Bluetooth Library - Bluetooth Monitor Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Monitor feature:
 * - Send real-time status messages to the mobile app
 * - Display system information and sensor readings
 * - Receive and process commands from the app
 * - Perfect for debugging and system monitoring
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
 * 2. Open Serial Monitor to see connection status
 * 3. Use DIYables Bluetooth App to connect and view monitor output
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothMonitor.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Monitor";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Monitor app instance
DIYables_BluetoothMonitor bluetoothMonitor;

// Variables for demo
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 3000;  // Send update every 3 seconds
int messageCount = 0;
bool ledState = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Monitor Example");
  
  // Initialize built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add monitor app to server
  bluetoothServer.addApp(&bluetoothMonitor);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothMonitor.send("=== Arduino Monitor Connected ===");
    bluetoothMonitor.send("System Ready");
    bluetoothMonitor.send("Type HELP for available commands");
    bluetoothMonitor.send("");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set up message handler for incoming commands
  bluetoothMonitor.onMonitorMessage([](const String& message) {
    Serial.print("Received command: ");
    Serial.println(message);
    
    handleCommand(message);
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void handleCommand(const String& cmd) {
  if (cmd == "HELP") {
    bluetoothMonitor.send("Available Commands:");
    bluetoothMonitor.send("  LED_ON     - Turn LED on");
    bluetoothMonitor.send("  LED_OFF    - Turn LED off");
    bluetoothMonitor.send("  STATUS     - Show system status");
    bluetoothMonitor.send("  CLEAR      - Clear monitor (if supported)");
    bluetoothMonitor.send("  HELP       - Show this help");
  }
  else if (cmd == "LED_ON") {
    digitalWrite(LED_BUILTIN, HIGH);
    ledState = true;
    bluetoothMonitor.send("✓ LED turned ON");
  }
  else if (cmd == "LED_OFF") {
    digitalWrite(LED_BUILTIN, LOW);
    ledState = false;
    bluetoothMonitor.send("✓ LED turned OFF");
  }
  else if (cmd == "STATUS") {
    showStatus();
  }
  else if (cmd == "CLEAR") {
    // App should handle clearing the display
    bluetoothMonitor.send("");
  }
  else {
    bluetoothMonitor.send("✗ Unknown command: " + cmd);
    bluetoothMonitor.send("Type HELP for available commands");
  }
}

void showStatus() {
  bluetoothMonitor.send("=== System Status ===");
  
  // LED Status
  bluetoothMonitor.send("LED State: " + String(ledState ? "ON" : "OFF"));
  
  // Uptime
  unsigned long uptime = millis() / 1000;
  bluetoothMonitor.send("Uptime: " + String(uptime / 3600) + "h " + String((uptime % 3600) / 60) + "m " + String(uptime % 60) + "s");
  
  // Free Memory (approximate for AVR)
  #ifdef __AVR__
  extern int __heap_start, *__brkval;
  int freeMemory = (int) &freeMemory - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  bluetoothMonitor.send("Free Memory: " + String(freeMemory) + " bytes");
  #endif
  
  // Messages sent
  bluetoothMonitor.send("Messages Sent: " + String(messageCount));
  
  bluetoothMonitor.send("====================");
}

void sendPeriodicUpdate() {
  messageCount++;
  
  // Example of different message types
  if (messageCount % 3 == 0) {
    // Send status update
    bluetoothMonitor.send("[INFO] Heartbeat #" + String(messageCount));
  } 
  else if (messageCount % 5 == 0) {
    // Send simulated sensor reading
    int sensorValue = random(0, 1024);
    bluetoothMonitor.send("[SENSOR] Reading: " + String(sensorValue) + " (random demo value)");
  }
  else {
    // Send timestamp
    bluetoothMonitor.send("[TIME] Uptime: " + String(millis() / 1000) + "s");
  }
  
  // Optionally log to Serial as well
  Serial.print("Sent update #");
  Serial.println(messageCount);
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send periodic updates (only when connected)
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    sendPeriodicUpdate();
  }
  
  delay(10);
}
