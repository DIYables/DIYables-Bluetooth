/*
 * DIYables Bluetooth Library - Bluetooth Plotter Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Plotter feature:
 * - Real-time data plotting via Bluetooth
 * - Plot multiple data series simultaneously
 * - Configurable plot settings
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
 * 3. Use DIYables Bluetooth App to connect and view the plot
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothPlotter.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Plotter";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Plotter app instance
DIYables_BluetoothPlotter bluetoothPlotter;

// Variables for generating sample data
unsigned long lastPlotTime = 0;
const unsigned long PLOT_INTERVAL = 100;  // Plot every 100ms
float phase = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Plotter Example");
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add plotter app to server
  bluetoothServer.addApp(&bluetoothPlotter);
  
  // Configure plotter settings
  bluetoothPlotter.setPlotTitle("Sensor Data");
  bluetoothPlotter.setAxisLabels("Time", "Value");
  bluetoothPlotter.setYAxisRange(-15, 30);  // Fixed range
  bluetoothPlotter.setMaxSamples(100);  // Show last 100 samples
  bluetoothPlotter.setLegendLabels("Sine", "Cosine", "Tangent");  // Custom legend labels
  
  // Uncomment to enable auto-scaling instead of fixed range:
  // bluetoothPlotter.enableAutoScale(true);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Optional: Handle requests for current plot data (when app loads)
  bluetoothPlotter.onDataRequest([]() {
    // Send initial data when app requests it
    Serial.println("App requested plot data");
    // Could send recent historical data here if needed
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send plot data at regular intervals
  if (millis() - lastPlotTime >= PLOT_INTERVAL) {
    lastPlotTime = millis();
    
    // Generate sample data (sine and cosine waves)
    float sine = sin(phase);
    float cosine = cos(phase);
    float tangent = tan(phase) * 0.3;  // Scaled down
    
    // Send data to Bluetooth plotter (multiple series)
    // You can send 1 to 4 values, or use array for more
    bluetoothPlotter.send(sine, cosine, tangent);
    
    // Also print to Serial for Arduino IDE Serial Plotter comparison
    // Format: value1 value2 value3 (space-separated for Serial Plotter)
    Serial.print(sine, 2);
    Serial.print(" ");
    Serial.print(cosine, 2);
    Serial.print(" ");
    Serial.println(tangent, 2);
    
    // Alternative methods:
    // bluetoothPlotter.send(sine);  // Single value
    // bluetoothPlotter.send(sine, cosine);  // Two values
    // float values[] = {sine, cosine, tangent};
    // bluetoothPlotter.send(values, 3);  // Array of values
    
    phase += 0.1;
    if (phase > 2 * PI) {
      phase = 0;
    }
    
    // TODO: Replace sample data with your actual sensor readings
    // Examples:
    // - Temperature sensor: bluetoothPlotter.send(temperature);
    // - Accelerometer: bluetoothPlotter.send(accelX, accelY, accelZ);
    // - Multiple sensors: bluetoothPlotter.send(sensor1, sensor2, sensor3);
  }
  
  delay(10);
}
