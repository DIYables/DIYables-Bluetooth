/*
 * DIYables Bluetooth Library - ESP32 BLE Plotter Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Plotter feature:
 * - Real-time data plotting via Bluetooth
 * - Plot multiple data series simultaneously
 * - Configurable plot settings
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
 * 3. Use DIYables Bluetooth App to connect and view the plot
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothPlotter.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Plotter";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Plotter app instance
DIYables_BluetoothPlotter bluetoothPlotter;

// Variables for generating sample data
unsigned long lastPlotTime = 0;
const unsigned long PLOT_INTERVAL = 100;
float phase = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Plotter Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add plotter app to server
  bluetoothServer.addApp(&bluetoothPlotter);
  
  // Configure plotter settings
  bluetoothPlotter.setPlotTitle("Sensor Data");
  bluetoothPlotter.setAxisLabels("Time", "Value");
  bluetoothPlotter.setYAxisRange(-15, 30);
  bluetoothPlotter.setMaxSamples(100);
  bluetoothPlotter.setLegendLabels("Sine", "Cosine", "Tangent");
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  bluetoothPlotter.onDataRequest([]() {
    Serial.println("App requested plot data");
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  bluetoothServer.loop();
  
  if (millis() - lastPlotTime >= PLOT_INTERVAL) {
    lastPlotTime = millis();
    
    float sine = sin(phase);
    float cosine = cos(phase);
    float tangent = tan(phase) * 0.3;
    
    bluetoothPlotter.send(sine, cosine, tangent);
    
    Serial.print(sine, 2);
    Serial.print(" ");
    Serial.print(cosine, 2);
    Serial.print(" ");
    Serial.println(tangent, 2);
    
    phase += 0.1;
    if (phase > 2 * PI) {
      phase = 0;
    }
  }
  
  delay(10);
}
