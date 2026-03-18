/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Plotter Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Plotter feature:
 * - Real-time data plotting via Bluetooth
 * - Plot multiple data series simultaneously
 * - Configurable plot settings
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
 * 3. Use DIYables Bluetooth App to connect and view the plot
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothPlotter.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Plotter");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Plotter app instance
DIYables_BluetoothPlotter bluetoothPlotter;

// Variables for generating sample data
unsigned long lastPlotTime = 0;
const unsigned long PLOT_INTERVAL = 100;  // Plot every 100ms
float phase = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Plotter Example");
  
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
    Serial.println("App requested plot data");
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
    bluetoothPlotter.send(sine, cosine, tangent);
    
    // Also print to Serial for Arduino IDE Serial Plotter comparison
    Serial.print(sine, 2);
    Serial.print(" ");
    Serial.print(cosine, 2);
    Serial.print(" ");
    Serial.println(tangent, 2);
    
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
