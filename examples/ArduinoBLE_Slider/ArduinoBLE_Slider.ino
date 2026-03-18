/*
 * DIYables Bluetooth Library - Bluetooth Slider Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Slider feature:
 * - Control values using sliders (0-100)
 * - Support for dual sliders
 * - Configurable range and step
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
 * 3. Use DIYables Bluetooth App to connect and control sliders
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothSlider.h>
#include <platforms/DIYables_ArduinoBLE.h>

// BLE Configuration
const char* DEVICE_NAME = "Arduino_Slider";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Slider app instance (min=0, max=100, step=1)
DIYables_BluetoothSlider bluetoothSlider(0, 100, 1);

// Variables to store current slider values
int currentSlider1 = 0;
int currentSlider2 = 0;

// PWM output pins (for LED brightness control example)
const int PWM_PIN_1 = 9;
const int PWM_PIN_2 = 10;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("DIYables Bluetooth - Slider Example");
  
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
    // Send initial slider positions
    bluetoothSlider.send(currentSlider1, currentSlider2);
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set up slider callback for value changes
  bluetoothSlider.onSliderValue([](int slider1, int slider2) {
    // Store the received values
    currentSlider1 = slider1;
    currentSlider2 = slider2;
    
    // Print slider values (0-100)
    Serial.print("Slider 1: ");
    Serial.print(slider1);
    Serial.print(", Slider 2: ");
    Serial.println(slider2);
    
    // Map slider values (0-100) to PWM range (0-255)
    int pwm1 = map(slider1, 0, 100, 0, 255);
    int pwm2 = map(slider2, 0, 100, 0, 255);
    
    // Control LED brightness
    analogWrite(PWM_PIN_1, pwm1);
    analogWrite(PWM_PIN_2, pwm2);
    
    // TODO: Add your control logic here based on slider values
    // Examples:
    // - Servo control: servo.write(map(slider1, 0, 100, 0, 180));
    // - Motor speed: analogWrite(MOTOR_PIN, pwm1);
    // - Volume control: setVolume(slider1);
    // - Brightness control: setBrightness(slider2);
  });
  
  // Optional: Handle requests for current slider values (when app loads)
  bluetoothSlider.onGetConfig([]() {
    // Send the stored slider values back to the app
    bluetoothSlider.send(currentSlider1, currentSlider2);
    Serial.print("App requested values - Sent: Slider1=");
    Serial.print(currentSlider1);
    Serial.print(", Slider2=");
    Serial.println(currentSlider2);
  });
  
  // You can change slider configuration at runtime:
  // bluetoothSlider.setRange(0, 255);  // Change range to 0-255
  // bluetoothSlider.setStep(5);        // Change step to 5 (coarser control)
  
  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Optional: Update slider positions based on sensor input
  // Example: Send current values periodically
  /*
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 5000) {
    lastUpdate = millis();
    bluetoothSlider.send(currentSlider1, currentSlider2);
  }
  */
  
  delay(10);
}
