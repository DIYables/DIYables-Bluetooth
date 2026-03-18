/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Rotator Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Rotator feature:
 * - Rotatable disc/knob control (0-360 degrees)
 * - Continuous or limited angle range
 * - Perfect for servo control, compass display, volume knobs
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
 * Optional: Servo motor for visual feedback
 * 
 * Setup:
 * 1. Upload the sketch to your ESP32
 * 2. Open Serial Monitor (115200 baud) to see connection status
 * 3. Use DIYables Bluetooth App to connect and rotate the knob
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothRotator.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Rotator");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Rotator app instance
// Option 1: Continuous mode (0-360, wraps around)
// DIYables_BluetoothRotator bluetoothRotator(ROTATOR_MODE_CONTINUOUS);

// Option 2: Limited mode (constrained angle range)
DIYables_BluetoothRotator bluetoothRotator(ROTATOR_MODE_LIMITED, 0, 180);

// Variables to store current angle
float currentAngle = 0.0;

// Optional: Servo control (uncomment if using ESP32Servo library)
// #include <ESP32Servo.h>
// Servo myServo;
// const int SERVO_PIN = 13;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Rotator Example");
  
  // Optional: Initialize servo
  // myServo.attach(SERVO_PIN);
  // myServo.write(0);
  
  bluetoothServer.begin();
  bluetoothServer.addApp(&bluetoothRotator);
  
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothRotator.send(currentAngle);
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  bluetoothRotator.onRotatorAngle([](float angle) {
    currentAngle = angle;
    
    Serial.print("Rotator angle: ");
    Serial.print(angle);
    Serial.println("°");
    
    // TODO: Add your control logic here based on angle
    // Examples:
    // - Servo control: myServo.write((int)angle);
    // - Stepper motor: stepper.moveTo(angleToSteps(angle));
    // - LED ring: setLEDPosition(angle);
    // - Volume control: setVolume(map(angle, 0, 360, 0, 100));
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  bluetoothServer.loop();
  delay(10);
}
