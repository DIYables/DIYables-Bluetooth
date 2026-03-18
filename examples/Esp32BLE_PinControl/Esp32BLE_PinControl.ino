/*
 * DIYables Bluetooth Library - ESP32 BLE Pin Control/Monitor Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth Pin Control/Monitor feature:
 * - Control digital output pins via Bluetooth
 * - Monitor digital input pins
 * - Monitor analog input pins
 * - Configure pin modes (INPUT/OUTPUT)
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
 * 3. Use DIYables Bluetooth App to connect and control pins
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothPinControl.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE_Pins";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create Pin Control/Monitor app instance
DIYables_BluetoothPinControl bluetoothPins;

// Pin configuration (ESP32 GPIOs)
const int LED_PIN = 2;       // Built-in LED
const int OUTPUT_PIN_1 = 16;
const int OUTPUT_PIN_2 = 17;
const int INPUT_PIN_1 = 25;
const int INPUT_PIN_2 = 26;
const int ANALOG_PIN_1 = 34;  // Input-only ADC pin
const int ANALOG_PIN_2 = 35;  // Input-only ADC pin

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - ESP32 BLE Pin Control/Monitor Example");
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(OUTPUT_PIN_1, OUTPUT);
  pinMode(OUTPUT_PIN_2, OUTPUT);
  pinMode(INPUT_PIN_1, INPUT_PULLUP);
  pinMode(INPUT_PIN_2, INPUT_PULLUP);
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add digital pins app to server
  bluetoothServer.addApp(&bluetoothPins);
  
  // Configure which pins are accessible via Bluetooth with custom names
  bluetoothPins.enablePin(LED_PIN, BT_PIN_OUTPUT, "LED");
  bluetoothPins.enablePin(OUTPUT_PIN_1, BT_PIN_OUTPUT, "Out1");
  bluetoothPins.enablePin(OUTPUT_PIN_2, BT_PIN_OUTPUT, "Out2");
  bluetoothPins.enablePin(INPUT_PIN_1, BT_PIN_INPUT, "Btn1");
  bluetoothPins.enablePin(INPUT_PIN_2, BT_PIN_INPUT, "Btn2");
  bluetoothPins.enablePin(ANALOG_PIN_1, BT_PIN_INPUT, "A34");
  bluetoothPins.enablePin(ANALOG_PIN_2, BT_PIN_INPUT, "A35");
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set up callback for pin write commands
  bluetoothPins.onPinWrite([](int pin, int state) {
    digitalWrite(pin, state);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" set to ");
    Serial.println(state ? "HIGH" : "LOW");
  });
  
  // Set up callback for pin read commands
  bluetoothPins.onPinRead([](int pin) -> int {
    int state;
    if (pin == ANALOG_PIN_1 || pin == ANALOG_PIN_2) {
      state = analogRead(pin);
      Serial.print("Analog pin ");
      Serial.print(pin);
      Serial.print(" read: ");
      Serial.println(state);
    } else {
      state = digitalRead(pin);
      Serial.print("Digital pin ");
      Serial.print(pin);
      Serial.print(" read: ");
      Serial.println(state ? "HIGH" : "LOW");
    }
    return state;
  });
  
  // Set up callback for pin mode changes
  bluetoothPins.onPinModeChange([](int pin, int mode) {
    pinMode(pin, mode == BT_PIN_OUTPUT ? OUTPUT : INPUT_PULLUP);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" mode changed to ");
    Serial.println(mode == BT_PIN_OUTPUT ? "OUTPUT" : "INPUT");
  });
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.print("Enabled pins: ");
  Serial.println(bluetoothPins.getEnabledPinCount());
}

void loop() {
  bluetoothServer.loop();
  
  static unsigned long lastInputCheck = 0;
  static int lastInputState1 = HIGH;
  static int lastInputState2 = HIGH;
  static int lastAnalogState1 = 0;
  static int lastAnalogState2 = 0;
  
  if (millis() - lastInputCheck >= 100) {
    lastInputCheck = millis();
    
    int currentState1 = digitalRead(INPUT_PIN_1);
    if (currentState1 != lastInputState1) {
      lastInputState1 = currentState1;
      bluetoothPins.updatePinState(INPUT_PIN_1, currentState1);
      Serial.print("Input pin ");
      Serial.print(INPUT_PIN_1);
      Serial.print(" changed to ");
      Serial.println(currentState1 ? "HIGH" : "LOW");
    }
    
    int currentState2 = digitalRead(INPUT_PIN_2);
    if (currentState2 != lastInputState2) {
      lastInputState2 = currentState2;
      bluetoothPins.updatePinState(INPUT_PIN_2, currentState2);
      Serial.print("Input pin ");
      Serial.print(INPUT_PIN_2);
      Serial.print(" changed to ");
      Serial.println(currentState2 ? "HIGH" : "LOW");
    }
    
    // ESP32 has 12-bit ADC (0-4095)
    int currentAnalog1 = analogRead(ANALOG_PIN_1);
    if (abs(currentAnalog1 - lastAnalogState1) > 40) {  // ~1% of 4095
      lastAnalogState1 = currentAnalog1;
      bluetoothPins.updatePinState(ANALOG_PIN_1, currentAnalog1);
      Serial.print("Analog pin ");
      Serial.print(ANALOG_PIN_1);
      Serial.print(" changed to ");
      Serial.println(currentAnalog1);
    }
    
    int currentAnalog2 = analogRead(ANALOG_PIN_2);
    if (abs(currentAnalog2 - lastAnalogState2) > 40) {
      lastAnalogState2 = currentAnalog2;
      bluetoothPins.updatePinState(ANALOG_PIN_2, currentAnalog2);
      Serial.print("Analog pin ");
      Serial.print(ANALOG_PIN_2);
      Serial.print(" changed to ");
      Serial.println(currentAnalog2);
    }
  }
  
  delay(10);
}
