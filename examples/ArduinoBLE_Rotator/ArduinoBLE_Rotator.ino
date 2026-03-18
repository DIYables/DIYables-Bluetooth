/*
 * DIYables Bluetooth Library - Bluetooth Rotator Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Rotator feature:
 * - Rotatable disc/knob control (0-360 degrees)
 * - Continuous or limited angle range
 * - Perfect for servo control, compass display, volume knobs
 * 
 * Compatible Boards:
 * - Arduino UNO R4 WiFi
 * - Arduino Nano 33 BLE / BLE Sense
 * - Arduino Nano 33 IoT
 * - Arduino MKR WiFi 1010
 * - Arduino Nano RP2040 Connect
 * - Any board supporting the ArduinoBLE library
 * 
 * Optional: Servo motor for visual feedback
 * 
 * Setup:
 * 1. Upload the sketch to your Arduino
 * 2. Open Serial Monitor to see connection status
 * 3. Use DIYables Bluetooth App to connect and rotate the knob
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothRotator.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Rotator";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Rotator app instance
// Option 1: Continuous mode (0-360, wraps around)
// DIYables_BluetoothRotator bluetoothRotator(ROTATOR_MODE_CONTINUOUS);

// Option 2: Limited mode (constrained angle range)
DIYables_BluetoothRotator bluetoothRotator(ROTATOR_MODE_LIMITED, 0, 180);

// Variables to store current angle
float currentAngle = 0.0;

// Optional: Servo control (uncomment if using Servo library)
// #include <Servo.h>
// Servo myServo;
// const int SERVO_PIN = 9;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Rotator Example");
  
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
    // - Servo control: servo.write(map(angle, 0, 360, 0, 180));
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
