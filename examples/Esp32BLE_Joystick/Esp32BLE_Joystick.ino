/*
 * DIYables Bluetooth Library - ESP32 BLE Joystick Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Joystick feature:
 * - Interactive joystick control via Bluetooth
 * - Real-time X/Y coordinate values (-100 to +100)
 * - Control pins based on joystick position
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
 * 3. Use DIYables Bluetooth App to connect and control the joystick
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothJoystick.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Joystick";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Joystick app instance
DIYables_BluetoothJoystick bluetoothJoystick(false, 5);

// Variables to store current joystick values
int currentJoystickX = 0;
int currentJoystickY = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Joystick Example");
  
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
    currentJoystickX = x;
    currentJoystickY = y;
    
    Serial.print("Joystick - X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.println(y);
    
    // TODO: Add your control logic here based on joystick position
  });
  
  bluetoothJoystick.onGetConfig([]() {
    bluetoothJoystick.send(currentJoystickX, currentJoystickY);
    Serial.print("App requested values - Sent: X=");
    Serial.print(currentJoystickX);
    Serial.print(", Y=");
    Serial.println(currentJoystickY);
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  bluetoothServer.loop();
  delay(10);
}
