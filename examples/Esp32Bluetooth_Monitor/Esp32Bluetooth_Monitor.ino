/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Monitor Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Monitor feature:
 * - Send real-time status messages to the mobile app
 * - Display system information and sensor readings
 * - Receive and process commands from the app
 * - Perfect for debugging and system monitoring
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
 * 2. Open Serial Monitor (115200 baud) to see connection status
 * 3. Use DIYables Bluetooth App to connect and view monitor output
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothMonitor.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Monitor");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Monitor app instance
DIYables_BluetoothMonitor bluetoothMonitor;

// Variables for demo
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 3000;  // Send update every 3 seconds
int messageCount = 0;
bool ledState = false;

// ESP32 built-in LED (may vary by board)
const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Monitor Example");
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add monitor app to server
  bluetoothServer.addApp(&bluetoothMonitor);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothMonitor.send("=== ESP32 Monitor Connected ===");
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
    bluetoothMonitor.send("  HEAP       - Show memory info");
    bluetoothMonitor.send("  CLEAR      - Clear monitor (if supported)");
    bluetoothMonitor.send("  HELP       - Show this help");
  }
  else if (cmd == "LED_ON") {
    digitalWrite(LED_PIN, HIGH);
    ledState = true;
    bluetoothMonitor.send("✓ LED turned ON");
  }
  else if (cmd == "LED_OFF") {
    digitalWrite(LED_PIN, LOW);
    ledState = false;
    bluetoothMonitor.send("✓ LED turned OFF");
  }
  else if (cmd == "STATUS") {
    showStatus();
  }
  else if (cmd == "HEAP") {
    bluetoothMonitor.send("=== Memory Info ===");
    bluetoothMonitor.send("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
    bluetoothMonitor.send("Min Free Heap: " + String(ESP.getMinFreeHeap()) + " bytes");
    bluetoothMonitor.send("Heap Size: " + String(ESP.getHeapSize()) + " bytes");
    bluetoothMonitor.send("===================");
  }
  else if (cmd == "CLEAR") {
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
  
  // ESP32-specific info
  bluetoothMonitor.send("Free Heap: " + String(ESP.getFreeHeap()) + " bytes");
  bluetoothMonitor.send("CPU Freq: " + String(ESP.getCpuFreqMHz()) + " MHz");
  bluetoothMonitor.send("Chip Model: " + String(ESP.getChipModel()));
  
  // Messages sent
  bluetoothMonitor.send("Messages Sent: " + String(messageCount));
  
  bluetoothMonitor.send("====================");
}

void sendPeriodicUpdate() {
  messageCount++;
  
  // Example of different message types
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
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send periodic updates (only when connected)
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    sendPeriodicUpdate();
  }
  
  delay(10);
}
