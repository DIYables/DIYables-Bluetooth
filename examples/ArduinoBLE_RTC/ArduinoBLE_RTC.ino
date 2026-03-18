/*
 * DIYables Bluetooth Library - Bluetooth RTC Example
 * Works with DIYables Bluetooth STEM app on Android and iOS
 * 
 * This example demonstrates the Bluetooth RTC (Real-Time Clock) feature:
 * - Real-time clock display for both Arduino and mobile app
 * - One-click time synchronization from mobile app to Arduino
 * - Hardware RTC integration for persistent timekeeping
 * - Visual time difference monitoring
 * 
 * Compatible Boards:
 * - Arduino UNO R4 WiFi (with built-in RTC)
 * Note: This example requires a board with hardware RTC.
 *       Other BLE boards can be used with an external RTC module (e.g., DS3231).
 * 
 * Setup:
 * 1. Upload the sketch to your Arduino
 * 2. Open Serial Monitor to see connection status
 * 3. Use DIYables Bluetooth App to connect and sync time
 * 
 * Tutorial: https://diyables.io/bluetooth-app
 * Author: DIYables
 */

#include <DIYables_BluetoothServer.h>
#include <DIYables_BluetoothRTC.h>
#include <platforms/DIYables_ArduinoBLE.h>
#include "RTC.h"

// BLE Configuration
const char* DEVICE_NAME = "Arduino_RTC";
const char* SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* TX_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214";
const char* RX_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214";

// Create Bluetooth instances
DIYables_ArduinoBLE bluetooth(DEVICE_NAME, SERVICE_UUID, TX_UUID, RX_UUID);
DIYables_BluetoothServer bluetoothServer(bluetooth);

// Create RTC app instance
DIYables_BluetoothRTC bluetoothRTC;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("DIYables Bluetooth - RTC Example");
  
  // Initialize RTC
  RTC.begin();

  // Check if RTC is running and set initial time if needed
  RTCTime savedTime;
  RTC.getTime(savedTime);

  if (!RTC.isRunning() || savedTime.getYear() == 2000) {
    Serial.println("RTC is NOT running, setting initial time...");
    // Set a default time - you can modify this to match current time
    RTCTime startTime(28, Month::AUGUST, 2025, 12, 0, 0, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);
    RTC.setTime(startTime);
    Serial.println("RTC initialized with default time");
  } else {
    Serial.println("RTC is already running");
  }

  // Print initial RTC time
  RTCTime initialTime;
  RTC.getTime(initialTime);
  Serial.print("Initial RTC Time: ");
  Serial.print(initialTime.getYear());
  Serial.print("/");
  Serial.print(Month2int(initialTime.getMonth()));
  Serial.print("/");
  Serial.print(initialTime.getDayOfMonth());
  Serial.print(" - ");
  if (initialTime.getHour() < 10) Serial.print("0");
  Serial.print(initialTime.getHour());
  Serial.print(":");
  if (initialTime.getMinutes() < 10) Serial.print("0");
  Serial.print(initialTime.getMinutes());
  Serial.print(":");
  if (initialTime.getSeconds() < 10) Serial.print("0");
  Serial.print(initialTime.getSeconds());
  Serial.println();
  
  // Initialize Bluetooth server with platform-specific implementation
  bluetoothServer.begin();
  
  // Add RTC app to server
  bluetoothServer.addApp(&bluetoothRTC);
  
  // Set up connection event callbacks
  bluetoothServer.setOnConnected([]() {
    Serial.println("Bluetooth connected!");
    // Send current time to app on connection
    sendCurrentTimeToApp();
  });
  
  bluetoothServer.setOnDisconnected([]() {
    Serial.println("Bluetooth disconnected!");
  });
  
  // Set callback for time sync from mobile app (Unix timestamp)
  bluetoothRTC.onTimeSync(onTimeSyncReceived);
  
  // Set callback for local time sync from mobile app (date/time components)
  bluetoothRTC.onLocalTimeSync(onLocalTimeSyncReceived);
  
  // Set callback for time request from mobile app
  bluetoothRTC.onTimeRequest(onTimeRequested);
  
  Serial.println("Waiting for Bluetooth connection...");
  Serial.println("Connect via app to sync time");
}

void loop() {
  // Handle Bluetooth server communications
  bluetoothServer.loop();
  
  // Send current time to mobile app and print to Serial every 1 second
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();

    // Get current RTC time
    RTCTime currentTime;
    RTC.getTime(currentTime);

    // Send time to mobile app in human readable format
    bluetoothRTC.sendTime(currentTime.getYear(), Month2int(currentTime.getMonth()),
                          currentTime.getDayOfMonth(), currentTime.getHour(),
                          currentTime.getMinutes(), currentTime.getSeconds());

    // Print time to Serial Monitor
    Serial.print("RTC Time: ");
    Serial.print(currentTime.getYear());
    Serial.print("/");
    Serial.print(Month2int(currentTime.getMonth()));
    Serial.print("/");
    Serial.print(currentTime.getDayOfMonth());
    Serial.print(" - ");
    if (currentTime.getHour() < 10) Serial.print("0");
    Serial.print(currentTime.getHour());
    Serial.print(":");
    if (currentTime.getMinutes() < 10) Serial.print("0");
    Serial.print(currentTime.getMinutes());
    Serial.print(":");
    if (currentTime.getSeconds() < 10) Serial.print("0");
    Serial.print(currentTime.getSeconds());
    Serial.println();
  }
  
  delay(10);
}

// Callback function called when mobile app sends time sync command
void onTimeSyncReceived(unsigned long unixTimestamp) {
  Serial.print("Time sync received (Unix): ");
  Serial.println(unixTimestamp);

  // Convert Unix timestamp to RTCTime
  RTCTime newTime;
  newTime.setUnixTime(unixTimestamp);

  // Set RTC time
  RTC.setTime(newTime);

  Serial.println("Arduino RTC synchronized from Unix timestamp!");
}

// Callback function called when mobile app sends local time sync with components
void onLocalTimeSyncReceived(int year, int month, int day, int hour, int minute, int second) {
  Serial.print("Local time sync received: ");
  Serial.print(year);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(day);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  // Create RTCTime from components (local time)
  // Convert month integer to Month enum
  Month monthEnum;
  switch(month) {
    case 1:  monthEnum = Month::JANUARY; break;
    case 2:  monthEnum = Month::FEBRUARY; break;
    case 3:  monthEnum = Month::MARCH; break;
    case 4:  monthEnum = Month::APRIL; break;
    case 5:  monthEnum = Month::MAY; break;
    case 6:  monthEnum = Month::JUNE; break;
    case 7:  monthEnum = Month::JULY; break;
    case 8:  monthEnum = Month::AUGUST; break;
    case 9:  monthEnum = Month::SEPTEMBER; break;
    case 10: monthEnum = Month::OCTOBER; break;
    case 11: monthEnum = Month::NOVEMBER; break;
    case 12: monthEnum = Month::DECEMBER; break;
    default: monthEnum = Month::JANUARY; break;
  }
  
  RTCTime newTime(day, monthEnum, year, hour, minute, second, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_ACTIVE);

  // Set RTC time
  RTC.setTime(newTime);

  Serial.println("Arduino RTC synchronized from local time components!");
}

// Callback function called when mobile app requests current Arduino time
void onTimeRequested() {
  Serial.println("Time requested by app");
  sendCurrentTimeToApp();
}

// Helper function to send current time to mobile app
void sendCurrentTimeToApp() {
  // Get current RTC time and send to app in human readable format
  RTCTime currentTime;
  RTC.getTime(currentTime);

  bluetoothRTC.sendTime(currentTime.getYear(), Month2int(currentTime.getMonth()),
                        currentTime.getDayOfMonth(), currentTime.getHour(),
                        currentTime.getMinutes(), currentTime.getSeconds());
}
