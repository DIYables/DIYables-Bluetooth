/*
 * DIYables Bluetooth Library - ESP32 BLE Multiple Apps Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates how to use multiple Bluetooth apps simultaneously:
 * - Monitor, Chat, Slider, Joystick, Temperature, Plotter,
 *   Table, Analog Gauge, Rotator
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
 * 2. Open Serial Monitor (115200 baud)
 * 3. Open DIYables Bluetooth App on your phone
 * 4. Connect to "ESP32BLE Multi-App"
 * 5. Navigate to different screens to test each app
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothMonitor.h>
#include <DIYables_BluetoothChat.h>
#include <DIYables_BluetoothSlider.h>
#include <DIYables_BluetoothJoystick.h>
#include <DIYables_BluetoothTemperature.h>
#include <DIYables_BluetoothPlotter.h>
#include <DIYables_BluetoothTable.h>
#include <DIYables_BluetoothAnalogGauge.h>
#include <DIYables_BluetoothRotator.h>
#include <platforms/DIYables_Esp32BLE.h>

// BLE Configuration
const char* DEVICE_NAME = "ESP32BLE Multi-App";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_Esp32BLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create app instances
DIYables_BluetoothMonitor      bluetoothMonitor;
DIYables_BluetoothChat         bluetoothChat;
DIYables_BluetoothSlider       bluetoothSlider(0, 255, 1);
DIYables_BluetoothJoystick     bluetoothJoystick(false, 5);
DIYables_BluetoothTemperature  bluetoothTemperature(-10.0, 50.0, "°C");
DIYables_BluetoothPlotter      bluetoothPlotter;
DIYables_BluetoothTable        bluetoothTable;
DIYables_BluetoothAnalogGauge  bluetoothGauge(0.0, 100.0, "%");
DIYables_BluetoothRotator      bluetoothRotator(ROTATOR_MODE_CONTINUOUS);

// State variables
int currentSlider1 = 128;
int currentSlider2 = 64;
int currentJoystickX = 0;
int currentJoystickY = 0;
float currentTemperature = 25.0;
float currentGaugeValue = 50.0;
float currentRotatorAngle = 0.0;
int messageCount = 0;

// Timing variables
unsigned long lastMonitorUpdate = 0;
unsigned long lastTempUpdate = 0;
unsigned long lastPlotUpdate = 0;
unsigned long lastTableUpdate = 0;
unsigned long lastGaugeUpdate = 0;
float plotPhase = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("DIYables Bluetooth - ESP32 BLE Multiple Apps Example");

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  bluetoothServer.begin();

  bluetoothServer.addApp(&bluetoothMonitor);
  bluetoothServer.addApp(&bluetoothChat);
  bluetoothServer.addApp(&bluetoothSlider);
  bluetoothServer.addApp(&bluetoothJoystick);
  bluetoothServer.addApp(&bluetoothTemperature);
  bluetoothServer.addApp(&bluetoothPlotter);
  bluetoothServer.addApp(&bluetoothTable);
  bluetoothServer.addApp(&bluetoothGauge);
  bluetoothServer.addApp(&bluetoothRotator);

  Serial.print("Registered apps: ");
  Serial.println(bluetoothServer.getAppCount());

  bluetoothPlotter.setPlotTitle("Sensor Data");
  bluetoothPlotter.setAxisLabels("Time", "Value");
  bluetoothPlotter.setYAxisRange(-1.5, 1.5);
  bluetoothPlotter.setMaxSamples(100);
  bluetoothPlotter.setLegendLabels("Sine", "Cosine", "Random");

  bluetoothTable.addRow("Status");
  bluetoothTable.addRow("Uptime");
  bluetoothTable.addRow("Slider 1");
  bluetoothTable.addRow("Slider 2");
  bluetoothTable.addRow("Joystick X");
  bluetoothTable.addRow("Joystick Y");
  bluetoothTable.addRow("Temperature");
  bluetoothTable.addRow("Gauge Value");
  bluetoothTable.addRow("Rotator Angle");
  bluetoothTable.addRow("Messages");

  setupCallbacks();

  Serial.println("Waiting for Bluetooth connection...");
}

void setupCallbacks() {
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    digitalWrite(2, HIGH);
    bluetoothMonitor.send("=== ESP32 BLE Multi-App Connected ===");
    bluetoothMonitor.send("All apps are ready!");
    bluetoothChat.send("Hello! ESP32 BLE Multi-App is connected.");
  });

  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
    digitalWrite(2, LOW);
  });

  bluetoothMonitor.onMonitorMessage([](const String& message) {
    Serial.println("Monitor cmd: " + message);
    if (message == "HELP") {
      bluetoothMonitor.send("Commands: STATUS, HELP, LED_ON, LED_OFF, HEAP");
    } else if (message == "STATUS") {
      bluetoothMonitor.send("Slider1=" + String(currentSlider1) + " Slider2=" + String(currentSlider2));
      bluetoothMonitor.send("Joystick X=" + String(currentJoystickX) + " Y=" + String(currentJoystickY));
      bluetoothMonitor.send("Temp=" + String(currentTemperature, 1) + "°C");
      bluetoothMonitor.send("Gauge=" + String(currentGaugeValue, 1) + "%");
      bluetoothMonitor.send("Rotator=" + String(currentRotatorAngle, 0) + "°");
    } else if (message == "LED_ON") {
      digitalWrite(2, HIGH);
      bluetoothMonitor.send("LED turned ON");
    } else if (message == "LED_OFF") {
      digitalWrite(2, LOW);
      bluetoothMonitor.send("LED turned OFF");
    } else if (message == "HEAP") {
      bluetoothMonitor.send("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
    } else {
      bluetoothMonitor.send("Unknown: " + message + " (type HELP)");
    }
  });

  bluetoothChat.onChatMessage([](const String& message) {
    Serial.println("Chat: " + message);
    bluetoothChat.send("Echo: " + message);
    if (message.equalsIgnoreCase("ping")) {
      bluetoothChat.send("pong!");
    } else if (message.equalsIgnoreCase("status")) {
      bluetoothChat.send("Uptime: " + String(millis() / 1000) + "s, Apps: " + String(bluetoothServer.getAppCount()));
    }
  });

  bluetoothSlider.onSliderValue([](int slider1, int slider2) {
    currentSlider1 = slider1;
    currentSlider2 = slider2;
    Serial.print("Slider 1: "); Serial.print(slider1);
    Serial.print(", Slider 2: "); Serial.println(slider2);
    currentGaugeValue = map(slider1, 0, 255, 0, 100);
    bluetoothGauge.send(currentGaugeValue);
    bluetoothTable.sendValueUpdate("Slider 1", String(slider1));
    bluetoothTable.sendValueUpdate("Slider 2", String(slider2));
    bluetoothTable.sendValueUpdate("Gauge Value", String(currentGaugeValue, 1) + "%");
  });

  bluetoothSlider.onGetConfig([]() {
    bluetoothSlider.send(currentSlider1, currentSlider2);
  });

  bluetoothJoystick.onJoystickValue([](int x, int y) {
    currentJoystickX = x;
    currentJoystickY = y;
    Serial.print("Joystick X: "); Serial.print(x);
    Serial.print(", Y: "); Serial.println(y);
    bluetoothTable.sendValueUpdate("Joystick X", String(x));
    bluetoothTable.sendValueUpdate("Joystick Y", String(y));
  });

  bluetoothJoystick.onGetConfig([]() {
    bluetoothJoystick.send(currentJoystickX, currentJoystickY);
  });

  bluetoothTemperature.onTemperatureRequest([]() {
    bluetoothTemperature.send(currentTemperature);
  });

  bluetoothPlotter.onDataRequest([]() {
    Serial.println("Plotter data requested");
  });

  bluetoothTable.onDataRequest([]() {
    Serial.println("Table data requested");
    bluetoothTable.sendTableStructure();
    updateAllTableValues();
  });

  bluetoothGauge.onValueRequest([]() {
    bluetoothGauge.send(currentGaugeValue);
  });

  bluetoothRotator.onRotatorAngle([](float angle) {
    currentRotatorAngle = angle;
    Serial.print("Rotator: "); Serial.print(angle); Serial.println("°");
    bluetoothTable.sendValueUpdate("Rotator Angle", String(angle, 0) + "°");
  });
}

void updateAllTableValues() {
  bluetoothTable.sendValueUpdate("Status", "Running");
  unsigned long uptime = millis() / 1000;
  String uptimeStr;
  if (uptime >= 60) {
    uptimeStr = String(uptime / 60) + "m " + String(uptime % 60) + "s";
  } else {
    uptimeStr = String(uptime) + "s";
  }
  bluetoothTable.sendValueUpdate("Uptime", uptimeStr);
  bluetoothTable.sendValueUpdate("Slider 1", String(currentSlider1));
  bluetoothTable.sendValueUpdate("Slider 2", String(currentSlider2));
  bluetoothTable.sendValueUpdate("Joystick X", String(currentJoystickX));
  bluetoothTable.sendValueUpdate("Joystick Y", String(currentJoystickY));
  bluetoothTable.sendValueUpdate("Temperature", String(currentTemperature, 1) + " °C");
  bluetoothTable.sendValueUpdate("Gauge Value", String(currentGaugeValue, 1) + "%");
  bluetoothTable.sendValueUpdate("Rotator Angle", String(currentRotatorAngle, 0) + "°");
  bluetoothTable.sendValueUpdate("Messages", String(messageCount));
}

void loop() {
  bluetoothServer.loop();

  if (!bluetooth.isConnected()) {
    delay(10);
    return;
  }

  if (millis() - lastMonitorUpdate >= 5000) {
    lastMonitorUpdate = millis();
    messageCount++;
    bluetoothMonitor.send("[INFO] Heartbeat #" + String(messageCount) + " - Uptime: " + String(millis() / 1000) + "s");
  }

  if (millis() - lastTempUpdate >= 2000) {
    lastTempUpdate = millis();
    static float tempOffset = 0;
    tempOffset += random(-10, 11) / 10.0;
    if (tempOffset > 5.0) tempOffset = 5.0;
    if (tempOffset < -5.0) tempOffset = -5.0;
    currentTemperature = 25.0 + tempOffset;
    bluetoothTemperature.send(currentTemperature);
    bluetoothTable.sendValueUpdate("Temperature", String(currentTemperature, 1) + " °C");
  }

  if (millis() - lastPlotUpdate >= 100) {
    lastPlotUpdate = millis();
    float sine = sin(plotPhase);
    float cosine = cos(plotPhase);
    float noise = random(-50, 51) / 100.0;
    bluetoothPlotter.send(sine, cosine, noise);
    plotPhase += 0.1;
    if (plotPhase > 2 * PI) plotPhase = 0;
  }

  if (millis() - lastTableUpdate >= 5000) {
    lastTableUpdate = millis();
    unsigned long uptime = millis() / 1000;
    String uptimeStr;
    if (uptime >= 60) {
      uptimeStr = String(uptime / 60) + "m " + String(uptime % 60) + "s";
    } else {
      uptimeStr = String(uptime) + "s";
    }
    bluetoothTable.sendValueUpdate("Uptime", uptimeStr);
    bluetoothTable.sendValueUpdate("Messages", String(messageCount));
  }

  if (millis() - lastGaugeUpdate >= 3000) {
    lastGaugeUpdate = millis();
    float sensorValue = 50.0 + 30.0 * sin(millis() / 10000.0);
    currentGaugeValue = sensorValue;
    bluetoothGauge.send(currentGaugeValue);
    bluetoothTable.sendValueUpdate("Gauge Value", String(currentGaugeValue, 1) + "%");
  }

  delay(10);
}
