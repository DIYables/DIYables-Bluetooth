/*
 * DIYables Bluetooth Library - ESP32 BLE Temperature Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Temperature feature:
 * - Display temperature sensor readings
 * - Configurable temperature range and unit
 * - Real-time temperature updates
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
 * Optional: Temperature sensor (DHT22, DS18B20, or analog thermistor)
 * 
 * Setup:
 * 1. Upload the sketch to your ESP32
 * 2. Open Serial Monitor (115200 baud) to see connection status
 * 3. Use DIYables Bluetooth App to connect and view temperature
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothTemperature.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Temp";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Temperature app instance (min=-10°C, max=50°C, unit="°C")
DIYables_BluetoothTemperature bluetoothTemperature(-10.0, 50.0, "°C");

// Variables for temperature simulation
float currentTemperature = 25.0;
unsigned long lastTempUpdate = 0;
const unsigned long TEMP_UPDATE_INTERVAL = 2000;

// Simulated temperature sensor reading
float readTemperature() {
  // TODO: Replace with actual sensor reading
  static float offset = 0;
  offset += random(-10, 11) / 10.0;
  if (offset > 5.0) offset = 5.0;
  if (offset < -5.0) offset = -5.0;
  return 25.0 + offset;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Temperature Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add temperature app to server
  bluetoothServer.addApp(&bluetoothTemperature);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    float temp = readTemperature();
    bluetoothTemperature.send(temp);
    Serial.print("Initial temperature sent: ");
    Serial.print(temp);
    Serial.println("°C");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  bluetoothTemperature.onTemperatureRequest([]() {
    float temp = readTemperature();
    bluetoothTemperature.send(temp);
    Serial.print("Temperature requested - Sent: ");
    Serial.print(temp);
    Serial.println("°C");
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  bluetoothServer.loop();
  
  if (bluetooth.isConnected() && millis() - lastTempUpdate >= TEMP_UPDATE_INTERVAL) {
    lastTempUpdate = millis();
    currentTemperature = readTemperature();
    bluetoothTemperature.send(currentTemperature);
    
    Serial.print("Temperature: ");
    Serial.print(currentTemperature);
    Serial.println("°C");
  }
  
  delay(10);
}
