/*
 * DIYables Bluetooth Library - ESP32 BLE Monitor Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Monitor feature:
 * - Send real-time status messages to the mobile app
 * - Display system information and sensor readings
 * - Receive and process commands from the app
 * - Perfect for debugging and system monitoring
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
 * 2. Open Serial Monitor (115200 baud) to see connection status
 * 3. Use DIYables Bluetooth App to connect and view monitor output
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothMonitor.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Monitor";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Monitor app instance
DIYables_BluetoothMonitor bluetoothMonitor;

// Variables for demo
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 3000;
int messageCount = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Monitor Example");
  
  // Initialize built-in LED
  pinMode(2, OUTPUT);  // ESP32 built-in LED is usually on GPIO 2
  digitalWrite(2, LOW);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add monitor app to server
  bluetoothServer.addApp(&bluetoothMonitor);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothMonitor.send("=== ESP32 BLE Monitor Connected ===");
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
    bluetoothMonitor.send("  HEAP       - Show free heap memory");
    bluetoothMonitor.send("  HELP       - Show this help");
  }
  else if (cmd == "LED_ON") {
    digitalWrite(2, HIGH);
    ledState = true;
    bluetoothMonitor.send("LED turned ON");
  }
  else if (cmd == "LED_OFF") {
    digitalWrite(2, LOW);
    ledState = false;
    bluetoothMonitor.send("LED turned OFF");
  }
  else if (cmd == "STATUS") {
    showStatus();
  }
  else if (cmd == "HEAP") {
    bluetoothMonitor.send("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  }
  else {
    bluetoothMonitor.send("Unknown command: " + cmd);
    bluetoothMonitor.send("Type HELP for available commands");
  }
}

void showStatus() {
  bluetoothMonitor.send("=== System Status ===");
  bluetoothMonitor.send("LED State: " + String(ledState ? "ON" : "OFF"));
  
  unsigned long uptime = millis() / 1000;
  bluetoothMonitor.send("Uptime: " + String(uptime / 3600) + "h " + String((uptime % 3600) / 60) + "m " + String(uptime % 60) + "s");
  bluetoothMonitor.send("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  bluetoothMonitor.send("Messages Sent: " + String(messageCount));
  bluetoothMonitor.send("====================");
}

void sendPeriodicUpdate() {
  messageCount++;
  
  if (messageCount % 3 == 0) {
    bluetoothMonitor.send("[INFO] Heartbeat #" + String(messageCount));
  } 
  else if (messageCount % 5 == 0) {
    bluetoothMonitor.send("[HEAP] Free: " + String(ESP.getFreeHeap()) + " bytes");
  }
  else {
    bluetoothMonitor.send("[TIME] Uptime: " + String(millis() / 1000) + "s");
  }
  
  Serial.print("Sent update #");
  Serial.println(messageCount);
}

void loop() {
  bluetoothServer.loop();
  
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    sendPeriodicUpdate();
  }
  
  delay(10);
}
