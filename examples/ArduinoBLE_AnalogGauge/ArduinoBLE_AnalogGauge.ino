/*
 * DIYables Bluetooth Library - Bluetooth Analog Gauge Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Analog Gauge feature:
 * - Display values on an analog meter/gauge
 * - Configurable range and unit
 * - Perfect for sensor monitoring (speed, pressure, voltage, etc.)
 * 
 * Compatible Boards:
 * - Arduino UNO R4 WiFi
 * - Arduino Nano 33 BLE / BLE Sense
 * - Arduino Nano 33 IoT
 * - Arduino MKR WiFi 1010
 * - Arduino Nano RP2040 Connect
 * - Any board supporting the ArduinoBLE library
 * 
 * Optional: Analog sensor (potentiometer, pressure sensor, etc.)
 * 
 * Setup:
 * 1. Upload the sketch to your Arduino
 * 2. Open Serial Monitor to see connection status
 * 3. Use DIYables Bluetooth App to connect and view the gauge
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothAnalogGauge.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Gauge";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Analog Gauge app instance (min=0, max=100, unit="km/h")
DIYables_BluetoothAnalogGauge bluetoothGauge(0.0, 100.0, "km/h");

// Variables for gauge value
float currentValue = 0.0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 200;  // Update every 200ms

// Optional: Analog input pin for sensor
const int ANALOG_PIN = A0;

// Function to read sensor value
float readSensorValue() {
  // TODO: Replace with actual sensor reading
  // Examples:
  // - Pressure sensor: readPressure()
  // - Voltage sensor: analogRead(A0) * (5.0 / 1023.0)
  // - Speed sensor: calculateSpeed()
  
  // Option 1: Read from analog pin and map to gauge range
  // int rawValue = analogRead(ANALOG_PIN);
  // return map(rawValue, 0, 1023, 0, 100);
  
  // Option 2: Simulated data (sine wave)
  static float phase = 0;
  phase += 0.05;
  if (phase > 2 * PI) phase = 0;
  return 50 + 50 * sin(phase);  // Oscillates between 0-100
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Analog Gauge Example");
  
  // Optional: Initialize analog pin
  // pinMode(ANALOG_PIN, INPUT);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add gauge app to server
  bluetoothServer.addApp(&bluetoothGauge);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    // Send initial value
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
  
  // Optional: Handle requests for current value
  bluetoothGauge.onValueRequest([]() {
    currentValue = readSensorValue();
    bluetoothGauge.send(currentValue);
    Serial.print("Value requested - Sent: ");
    Serial.print(currentValue);
    Serial.print(" ");
    Serial.println(bluetoothGauge.getUnit());
  });
  
  // You can change gauge configuration at runtime:
  // bluetoothGauge.setRange(0.0, 200.0);  // Change range to 0-200
  // bluetoothGauge.setUnit("mph");        // Change unit to mph
  // bluetoothGauge.setRange(0.0, 5.0);    // For voltage (0-5V)
  // bluetoothGauge.setUnit("V");
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.print("Gauge range: ");
  Serial.print(bluetoothGauge.getMin());
  Serial.print(" - ");
  Serial.print(bluetoothGauge.getMax());
  Serial.print(" ");
  Serial.println(bluetoothGauge.getUnit());
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send gauge updates periodically (only when connected)
  if (bluetooth.isConnected() && millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    
    // Read sensor value
    currentValue = readSensorValue();
    
    // Send to Bluetooth app
    bluetoothGauge.send(currentValue);
    
    // Print to Serial Monitor
    Serial.print("Gauge: ");
    Serial.print(currentValue, 1);
    Serial.print(" ");
    Serial.println(bluetoothGauge.getUnit());
  }
  
  delay(10);
}
