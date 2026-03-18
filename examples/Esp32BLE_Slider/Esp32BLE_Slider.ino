/*
 * DIYables Bluetooth Library - ESP32 BLE Slider Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Slider feature:
 * - Control values using sliders (0-100)
 * - Support for dual sliders
 * - Configurable range and step
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
 * 3. Use DIYables Bluetooth App to connect and control sliders
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothSlider.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Slider";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Slider app instance (min=0, max=100, step=1)
DIYables_BluetoothSlider bluetoothSlider(0, 100, 1);

// Variables to store current slider values
int currentSlider1 = 0;
int currentSlider2 = 0;

// PWM output pins
const int PWM_PIN_1 = 16;
const int PWM_PIN_2 = 17;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Slider Example");
  
  // Initialize PWM pins
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add slider app to server
  bluetoothServer.addApp(&bluetoothSlider);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    bluetoothSlider.send(currentSlider1, currentSlider2);
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set up slider callback for value changes
  bluetoothSlider.onSliderValue([](int slider1, int slider2) {
    currentSlider1 = slider1;
    currentSlider2 = slider2;
    
    Serial.print("Slider 1: ");
    Serial.print(slider1);
    Serial.print(", Slider 2: ");
    Serial.println(slider2);
    
    // Map slider values (0-100) to PWM range (0-255)
    int pwm1 = map(slider1, 0, 100, 0, 255);
    int pwm2 = map(slider2, 0, 100, 0, 255);
    
    analogWrite(PWM_PIN_1, pwm1);
    analogWrite(PWM_PIN_2, pwm2);
    
    // TODO: Add your control logic here
  });
  
  bluetoothSlider.onGetConfig([]() {
    bluetoothSlider.send(currentSlider1, currentSlider2);
    Serial.print("App requested values - Sent: Slider1=");
    Serial.print(currentSlider1);
    Serial.print(", Slider2=");
    Serial.println(currentSlider2);
  });
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  bluetoothServer.loop();
  delay(10);
}
