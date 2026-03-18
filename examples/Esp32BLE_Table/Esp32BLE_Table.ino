/*
 * DIYables Bluetooth Library - ESP32 BLE Table Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Table feature:
 * - Display structured data in a two-column table
 * - Real-time value updates for each row
 * - Perfect for sensor dashboards and status displays
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
 * 3. Use DIYables Bluetooth App to connect and view the table
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothTable.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Table";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Table app instance
DIYables_BluetoothTable bluetoothTable;

// Variables for demo data
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000;
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Table Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add table app to server
  bluetoothServer.addApp(&bluetoothTable);
  
  // Define table structure
  bluetoothTable.addRow("Temperature");
  bluetoothTable.addRow("Humidity");
  bluetoothTable.addRow("Pressure");
  bluetoothTable.addRow("Counter");
  bluetoothTable.addRow("Uptime");
  bluetoothTable.addRow("Free Heap");
  bluetoothTable.addRow("Status");
  
  Serial.print("Table rows defined: ");
  Serial.println(bluetoothTable.getRowCount());
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothTable.sendTableStructure();
    updateTableValues();
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  bluetoothTable.onDataRequest([]() {
    Serial.println("App requested table data");
    bluetoothTable.sendTableStructure();
    updateTableValues();
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void updateTableValues() {
  float temperature = 20.0 + random(0, 100) / 10.0;
  bluetoothTable.sendValueUpdate("Temperature", String(temperature, 1) + " °C");
  
  int humidity = 40 + random(0, 21);
  bluetoothTable.sendValueUpdate("Humidity", String(humidity) + " %");
  
  int pressure = 1000 + random(0, 21);
  bluetoothTable.sendValueUpdate("Pressure", String(pressure) + " hPa");
  
  bluetoothTable.sendValueUpdate("Counter", String(counter));
  counter++;
  
  unsigned long uptime = millis() / 1000;
  String uptimeStr = String(uptime / 3600) + "h " + 
                     String((uptime % 3600) / 60) + "m " + 
                     String(uptime % 60) + "s";
  bluetoothTable.sendValueUpdate("Uptime", uptimeStr);
  
  bluetoothTable.sendValueUpdate("Free Heap", String(ESP.getFreeHeap()) + " bytes");
  
  bluetoothTable.sendValueUpdate("Status", counter % 2 == 0 ? "Running" : "Active");
  
  Serial.println("Table values updated");
}

void loop() {
  bluetoothServer.loop();
  
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    updateTableValues();
  }
  
  delay(10);
}
