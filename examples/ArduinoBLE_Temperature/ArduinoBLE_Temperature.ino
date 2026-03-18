/*
 * DIYables Bluetooth Library - Bluetooth Temperature Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Temperature feature:
 * - Display temperature sensor readings
 * - Configurable temperature range and unit
 * - Real-time temperature updates
 * 
 * Compatible Boards:
 * - Arduino UNO R4 WiFi
 * - Arduino Nano 33 BLE / BLE Sense
 * - Arduino Nano 33 IoT
 * - Arduino MKR WiFi 1010
 * - Arduino Nano RP2040 Connect
 * - Any board supporting the ArduinoBLE library
 * 
 * Optional: Temperature sensor (DHT22, DS18B20, or analog thermistor)
 * 
 * Setup:
 * 1. Upload the sketch to your Arduino
 * 2. Open Serial Monitor to see connection status
 * 3. Use DIYables Bluetooth App to connect and view temperature
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothTemperature.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Temp";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
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
  // - Analog thermistor: analogToTemperature(analogRead(A0))
  
  // Simulate temperature changes
  static float offset = 0;
  offset += random(-10, 11) / 10.0;  // Random walk
  if (offset > 5.0) offset = 5.0;
  if (offset < -5.0) offset = -5.0;
  
  return 25.0 + offset;  // Base temperature 25°C with variation
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Temperature Example");
  
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
