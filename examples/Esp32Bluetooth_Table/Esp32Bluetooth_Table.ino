/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Table Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Table feature:
 * - Display structured data in a two-column table
 * - Real-time value updates for each row
 * - Perfect for sensor dashboards and status displays
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
 * 3. Use DIYables Bluetooth App to connect and view the table
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothTable.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Table");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Table app instance
DIYables_BluetoothTable bluetoothTable;

// Variables for demo data
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000;  // Update every second
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Table Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add table app to server
  bluetoothServer.addApp(&bluetoothTable);
  
  // Define table structure (add rows with attribute names)
  bluetoothTable.addRow("Temperature");
  bluetoothTable.addRow("Humidity");
  bluetoothTable.addRow("Pressure");
  bluetoothTable.addRow("Counter");
  bluetoothTable.addRow("Uptime");
  bluetoothTable.addRow("Free Heap");
  bluetoothTable.addRow("CPU Freq");
  bluetoothTable.addRow("Status");
  
  Serial.print("Table rows defined: ");
  Serial.println(bluetoothTable.getRowCount());
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    // Send table structure
    bluetoothTable.sendTableStructure();
    // Send initial values
    updateTableValues();
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Optional: Handle requests for table data
  bluetoothTable.onDataRequest([]() {
    Serial.println("App requested table data");
    bluetoothTable.sendTableStructure();
    updateTableValues();
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void updateTableValues() {
  // TODO: Replace with actual sensor readings
  
  // Simulated temperature (20-30°C)
  float temperature = 20.0 + random(0, 100) / 10.0;
  bluetoothTable.sendValueUpdate("Temperature", String(temperature, 1) + " °C");
  
  // Simulated humidity (40-60%)
  int humidity = 40 + random(0, 21);
  bluetoothTable.sendValueUpdate("Humidity", String(humidity) + " %");
  
  // Simulated pressure (1000-1020 hPa)
  int pressure = 1000 + random(0, 21);
  bluetoothTable.sendValueUpdate("Pressure", String(pressure) + " hPa");
  
  // Counter value
  bluetoothTable.sendValueUpdate("Counter", String(counter));
  counter++;
  
  // Uptime (in seconds)
  unsigned long uptime = millis() / 1000;
  String uptimeStr = String(uptime / 3600) + "h " + 
                     String((uptime % 3600) / 60) + "m " + 
                     String(uptime % 60) + "s";
  bluetoothTable.sendValueUpdate("Uptime", uptimeStr);
  
  // ESP32-specific: Free heap memory
  bluetoothTable.sendValueUpdate("Free Heap", String(ESP.getFreeHeap()) + " bytes");
  
  // ESP32-specific: CPU frequency
  bluetoothTable.sendValueUpdate("CPU Freq", String(ESP.getCpuFreqMHz()) + " MHz");
  
  // Status
  bluetoothTable.sendValueUpdate("Status", counter % 2 == 0 ? "Running" : "Active");
  
  Serial.println("Table values updated");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Update table values periodically (only when connected)
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    updateTableValues();
  }
  
  delay(10);
}
