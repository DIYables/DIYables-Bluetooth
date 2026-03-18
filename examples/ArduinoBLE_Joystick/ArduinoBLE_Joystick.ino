/*
 * DIYables Bluetooth Library - Bluetooth Joystick Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Joystick feature:
 * - Interactive joystick control via Bluetooth
 * - Real-time X/Y coordinate values (-100 to +100)
 * - Control pins based on joystick position
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
 * 3. Use DIYables Bluetooth App to connect and control the joystick
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothJoystick.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Joystick";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Joystick app instance
// Configure with autoReturn=false and sensitivity=5 (minimum 5% change to trigger updates)
DIYables_BluetoothJoystick bluetoothJoystick(false, 5);

// Variables to store current joystick values
int currentJoystickX = 0;
int currentJoystickY = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Joystick Example");
  
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
    // - Control LEDs: analogWrite(LED_PIN, map(abs(x), 0, 100, 0, 255));
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
