/*
 * DIYables Bluetooth Library - Bluetooth Table Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Table feature:
 * - Display structured data in a two-column table
 * - Real-time value updates for each row
 * - Perfect for sensor dashboards and status displays
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
 * 3. Use DIYables Bluetooth App to connect and view the table
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothTable.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Table";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Table app instance
DIYables_BluetoothTable bluetoothTable;

// Variables for demo data
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000;  // Update every second
int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Table Example");
  
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
  bluetoothTable.addRow("Free Memory");
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
  
  // Free memory (simulated)
  int freeMemory = 2048 - random(0, 512);
  bluetoothTable.sendValueUpdate("Free Memory", String(freeMemory) + " bytes");
  
  // Status
  bluetoothTable.sendValueUpdate("Status", counter % 2 == 0 ? "Running" : "Active");
  
  // Alternative: Update by index instead of attribute name
  // bluetoothTable.sendValueUpdate(0, String(temperature, 1) + " °C");
  // bluetoothTable.sendValueUpdate(1, String(humidity) + " %");
  
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
