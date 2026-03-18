/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Temperature Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Temperature feature:
 * - Display temperature sensor readings
 * - Configurable temperature range and unit
 * - Real-time temperature updates
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
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Temp");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Temperature app instance (min=-10°C, max=50°C, unit="°C")
DIYables_BluetoothTemperature bluetoothTemperature(-10.0, 50.0, "°C");

// Variables for temperature simulation
float currentTemperature = 25.0;
unsigned long lastTempUpdate = 0;
const unsigned long TEMP_UPDATE_INTERVAL = 2000;  // Update every 2 seconds

// Simulated temperature sensor reading
float readTemperature() {
  // TODO: Replace with actual sensor reading
  // Examples:
  // - DHT22: dht.readTemperature()
  // - DS18B20: sensors.getTempCByIndex(0)
  // - Analog thermistor: analogToTemperature(analogRead(34))
  // - ESP32 internal temp: temperatureRead() (approximate)
  
  // Simulate temperature changes
  static float offset = 0;
  offset += random(-10, 11) / 10.0;  // Random walk
  if (offset > 5.0) offset = 5.0;
  if (offset < -5.0) offset = -5.0;
  
  return 25.0 + offset;  // Base temperature 25°C with variation
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Temperature Example");
  
  // TODO: Initialize your temperature sensor here
  // Examples:
  // dht.begin();
  // sensors.begin();
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add temperature app to server
  bluetoothServer.addApp(&bluetoothTemperature);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    // Send initial temperature reading
    float temp = readTemperature();
    bluetoothTemperature.send(temp);
    Serial.print("Initial temperature sent: ");
    Serial.print(temp);
    Serial.println("°C");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Optional: Handle requests for temperature value
  bluetoothTemperature.onTemperatureRequest([]() {
    float temp = readTemperature();
    bluetoothTemperature.send(temp);
    Serial.print("Temperature requested - Sent: ");
    Serial.print(temp);
    Serial.println("°C");
  });
  
  // You can change temperature configuration at runtime:
  // bluetoothTemperature.setRange(-40.0, 125.0);  // Wide range for industrial sensors
  // bluetoothTemperature.setUnit("°F");            // Change to Fahrenheit
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send temperature updates periodically (only when connected)
  if (bluetooth.isConnected() && millis() - lastTempUpdate >= TEMP_UPDATE_INTERVAL) {
    lastTempUpdate = millis();
    
    // Read temperature from sensor
    currentTemperature = readTemperature();
    
    // Send to Bluetooth app
    bluetoothTemperature.send(currentTemperature);
    
    // Print to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(currentTemperature);
    Serial.println("°C");
  }
  
  delay(10);
}
