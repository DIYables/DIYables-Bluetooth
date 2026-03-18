/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Joystick Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Joystick feature:
 * - Interactive joystick control via Bluetooth
 * - Real-time X/Y coordinate values (-100 to +100)
 * - Control pins based on joystick position
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
 * 3. Use DIYables Bluetooth App to connect and control the joystick
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothJoystick.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Joystick");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Joystick app instance
// Configure with autoReturn=false and sensitivity=5 (minimum 5% change to trigger updates)
DIYables_BluetoothJoystick bluetoothJoystick(false, 5);

// Variables to store current joystick values
int currentJoystickX = 0;
int currentJoystickY = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Joystick Example");
  
  // TODO: initialize your hardware pins here
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add joystick app to server
  bluetoothServer.addApp(&bluetoothJoystick);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set up joystick callback for position changes
  bluetoothJoystick.onJoystickValue([](int x, int y) {
    // Store the received values
    currentJoystickX = x;
    currentJoystickY = y;
    
    // Print joystick position values (-100 to +100)
    Serial.print("Joystick - X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.println(y);
    
    // TODO: Add your control logic here based on joystick position
    // Examples:
    // - Control motors: if (x > 50) { /* move right */ }
    // - Control servos: servo.write(map(y, -100, 100, 0, 180));
    // - Control LEDs: ledcWrite(channel, map(abs(x), 0, 100, 0, 255));
    // - Send commands to other devices via Serial, I2C, SPI, etc.
  });
  
  // Optional: Handle requests for current joystick values (when app loads)
  bluetoothJoystick.onGetConfig([]() {
    // Send the stored joystick values back to the app
    bluetoothJoystick.send(currentJoystickX, currentJoystickY);
    Serial.print("App requested values - Sent: X=");
    Serial.print(currentJoystickX);
    Serial.print(", Y=");
    Serial.println(currentJoystickY);
  });
  
  // You can change configuration at runtime:
  // bluetoothJoystick.setAutoReturn(false);  // Disable auto-return
  // bluetoothJoystick.setSensitivity(10.0);  // Only send updates when joystick moves >10% (less sensitive)
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // TODO: Add your main application code here
  
  delay(10);
}
