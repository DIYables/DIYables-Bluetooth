/*
 * DIYables Bluetooth Library - ESP32 BLE Analog Gauge Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Analog Gauge feature:
 * - Display values on an analog meter/gauge
 * - Configurable range and unit
 * - Perfect for sensor monitoring (speed, pressure, voltage, etc.)
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
 * Optional: Analog sensor (potentiometer, pressure sensor, etc.)
 * 
 * Setup:
 * 1. Upload the sketch to your ESP32
 * 2. Open Serial Monitor (115200 baud) to see connection status
 * 3. Use DIYables Bluetooth App to connect and view the gauge
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothAnalogGauge.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Gauge";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Analog Gauge app instance (min=0, max=100, unit="km/h")
DIYables_BluetoothAnalogGauge bluetoothGauge(0.0, 100.0, "km/h");

// Variables for gauge value
float currentValue = 0.0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 200;  // Update every 200ms

// Optional: Analog input pin for sensor
const int ANALOG_PIN = 34;  // ESP32 ADC pin

// Function to read sensor value
float readSensorValue() {
  // TODO: Replace with actual sensor reading
  // Option 1: Read from analog pin and map to gauge range
  // int rawValue = analogRead(ANALOG_PIN);
  // return map(rawValue, 0, 4095, 0, 100);  // ESP32 has 12-bit ADC
  
  // Option 2: Simulated data (sine wave)
  static float phase = 0;
  phase += 0.05;
  if (phase > 2 * PI) phase = 0;
  return 50 + 50 * sin(phase);  // Oscillates between 0-100
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Analog Gauge Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add gauge app to server
  bluetoothServer.addApp(&bluetoothGauge);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    currentValue = readSensorValue();
    bluetoothGauge.send(currentValue);
    Serial.print("Initial value sent: ");
    Serial.print(currentValue);
    Serial.print(" ");
    Serial.println(bluetoothGauge.getUnit());
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  bluetoothGauge.onValueRequest([]() {
    currentValue = readSensorValue();
    bluetoothGauge.send(currentValue);
    Serial.print("Value requested - Sent: ");
    Serial.print(currentValue);
    Serial.print(" ");
    Serial.println(bluetoothGauge.getUnit());
  });
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.print("Gauge range: ");
  Serial.print(bluetoothGauge.getMin());
  Serial.print(" - ");
  Serial.print(bluetoothGauge.getMax());
  Serial.print(" ");
  Serial.println(bluetoothGauge.getUnit());
}

void loop() {
  bluetoothServer.loop();
  
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    currentValue = readSensorValue();
    bluetoothGauge.send(currentValue);
    
    Serial.print("Gauge: ");
    Serial.print(currentValue, 1);
    Serial.print(" ");
    Serial.println(bluetoothGauge.getUnit());
  }
  
  delay(10);
}
