/*
 * DIYables Bluetooth Library - ESP32 Classic Bluetooth Slider Example
 * Works with DIYables Bluetooth STEM app on Android
 * Note: Classic Bluetooth is NOT supported on iOS. Use BLE examples for iOS support.
 * 
 * This example demonstrates the Bluetooth Slider feature:
 * - Control values using sliders (0-100)
 * - Support for dual sliders
 * - Configurable range and step
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
 * 3. Use DIYables Bluetooth App to connect and control sliders
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothSlider.h>
#include <platforms/DIYables_Esp32Bluetooth.h>

// Create Bluetooth instances
DIYables_Esp32Bluetooth bluetooth("ESP32_Slider");
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Slider app instance (min=0, max=100, step=1)
DIYables_BluetoothSlider bluetoothSlider(0, 100, 1);

// Variables to store current slider values
int currentSlider1 = 0;
int currentSlider2 = 0;

// PWM output pins (for LED brightness control example)
const int PWM_PIN_1 = 16;
const int PWM_PIN_2 = 17;

// ESP32 LEDC PWM channels
const int PWM_CHANNEL_1 = 0;
const int PWM_CHANNEL_2 = 1;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 Slider Example");
  
  // Initialize PWM using ESP32 LEDC
  ledcSetup(PWM_CHANNEL_1, 5000, 8);  // 5kHz, 8-bit resolution
  ledcSetup(PWM_CHANNEL_2, 5000, 8);
  ledcAttachPin(PWM_PIN_1, PWM_CHANNEL_1);
  ledcAttachPin(PWM_PIN_2, PWM_CHANNEL_2);
  
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
    
    // Control LED brightness using ESP32 LEDC
    ledcWrite(PWM_CHANNEL_1, pwm1);
    ledcWrite(PWM_CHANNEL_2, pwm2);
    
    // TODO: Add your control logic here based on slider values
    // Examples:
    // - Servo control: myServo.write(map(slider1, 0, 100, 0, 180));
    // - Motor speed: ledcWrite(MOTOR_CHANNEL, pwm1);
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
  
  delay(10);
}
