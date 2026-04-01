# DIYables Bluetooth Library

A comprehensive Bluetooth communication library for Arduino and ESP32 that communicates with the **DIYables Bluetooth STEM** mobile app on Android and iOS.

![DIYables Bluetooth STEM App - Arduino ESP32 Bluetooth BLE Control App for Android and iOS](https://diyables.io/images/products/bluetooth-app.jpg)

📲 **Download the app:**
- **Android**: [Google Play Store](https://play.google.com/store/apps/details?id=io.diyables.bluetoothstem)
- **iOS**: [App Store](https://apps.apple.com/us/app/diyables-bluetooth-stem/id6760932195)

This library provides clean architecture with platform abstraction supporting both BLE (Bluetooth Low Energy) and Classic Bluetooth. Upload an Arduino sketch, connect via the mobile app, and start interacting with your hardware project — no mobile coding required.

## 🚀 Features

- **Multiple App Types**: Joystick, Plotter, Pin Control/Monitor, Chat, and Monitor
- **Clean Architecture**: SOLID principles with platform abstraction layer
- **Extensible Design**: Easy to add custom app types
- **BLE & Classic Support**: Works with ArduinoBLE, ESP32, and more
- **Ready Examples**: Complete working examples for each app type
- **Type-Safe API**: Clear interfaces with callback-based event handling

## 📱 Supported Hardware

| Board | BLE | Classic Bluetooth | Tested |
|---|---|---|---|
| Arduino Uno R4 WiFi | ✅ | ❌ | ✅ |
| [DIYables STEM V4 IoT Board (works like Arduino Uno R4 WiFi)](https://diyables.io/products/diyables-stem-v4-iot-fully-compatible-with-arduino-uno-r4-wifi) | ✅ | ❌ | ✅ |
| [DIYables STEM V4B IoT Board (works like Arduino Uno R4 WiFi)](https://diyables.io/products/diyables-stem-v4b-iot-development-board-compatible-with-arduino-uno-r4-wifi-ra4m1-32-bit-arm-cortex-m4-with-esp32-s3-wifi-bluetooth-usb-c-for-learning-prototyping-education) | ✅ | ❌ | ✅ |
| Arduino Giga | ✅ | ❌ | ✅ |
| [DIYables ESP32](https://diyables.io/products/38-pin-esp32s-esp-wroom-32-wifi-and-bluetooth-development-board-with-usb-type-c-and-cp2102-dual-core-esp32-microcontroller-for-iot-projects-compatible-with-arduino-ide) | ✅ | ✅ | ✅ |
| [DIYables ESP32 Uno-form](https://diyables.io/products/esp32-wroom-32-development-board-compatible-with-uno-form-factor-shields-wifi-bluetooth-ch340-usb-type-c-4mb-flash) | ✅ | ✅ | ✅ |
| [DIYables ESP32-S3 Uno-form](https://diyables.io/products/esp32-s3-development-board-with-esp32-s3-wroom-1-n16r8-wifi-bluetooth-uno-compatible-form-factor-works-with-arduino-ide) | ✅ | ✅ | ✅ |
| Arduino Nano 33 BLE | ✅ | ❌ | Not yet |
| Arduino Nano 33 BLE Sense | ✅ | ❌ | Not yet |
| Arduino Nano 33 IoT | ✅ | ❌ | ✅ |
| Arduino Nano ESP32 | ✅ | ✅ | Not yet |
| Arduino Nano RP2040 Connect | ✅ | ❌ | Not yet |
| ESP32-S3 | ✅ | ✅ | Not yet |
| ESP32-C3 | ✅ | ❌ | Not yet |
| Any ArduinoBLE-compatible board | ✅ | ❌ | Not yet |

## 📋 Available Apps

- 📊 **Monitor** — Real-time serial monitor, view Bluetooth messages and send commands
- 💬 **Chat** — Interactive chat interface with Arduino
- 🔌 **Digital Pin Control** — Control and monitor digital/analog pins
- 🕹️ **Joystick** — Two-axis joystick for robotics and vehicle control
- 🎚️ **Sliders** — Dual analog sliders for PWM and analog control
- 📈 **Plotter** — Real-time data visualization with up to 8 data series
- ⏲️ **Analog Gauge** — Circular gauge for sensor monitoring
- 🔄 **Rotator** — Rotary angle control for servos and stepper motors
- 🌡️ **Temperature** — Visual thermometer display for temperature sensors
- 🕐 **RTC** — Real-time clock synchronization between phone and Arduino
- 📋 **Data Table** — Structured attribute-value data display

## 📖 Tutorials

- [DIYables Bluetooth STEM App User Manual](https://diyables.io/bluetooth-app)

### Arduino UNO R4 WiFi

- [Arduino UNO R4 WiFi - Bluetooth Analog Gauge](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-analog-gauge)
- [Arduino UNO R4 WiFi - Bluetooth Chat](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-chat)
- [Arduino UNO R4 WiFi - Bluetooth Digital Pins](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-digital-pins)
- [Arduino UNO R4 WiFi - Bluetooth Joystick](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-joystick)
- [Arduino UNO R4 WiFi - Bluetooth Monitor](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-monitor)
- [Arduino UNO R4 WiFi - Bluetooth Multiple Apps](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-multiple-apps)
- [Arduino UNO R4 WiFi - Bluetooth Plotter](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-plotter)
- [Arduino UNO R4 WiFi - Bluetooth Rotator](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-rotator)
- [Arduino UNO R4 WiFi - Bluetooth RTC](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-rtc)
- [Arduino UNO R4 WiFi - Bluetooth Slider](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-slider)
- [Arduino UNO R4 WiFi - Bluetooth Table](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-table)
- [Arduino UNO R4 WiFi - Bluetooth Temperature](https://newbiely.com/tutorials/arduino-uno-r4/arduino-uno-r4-diyables-bluetooth-app-temperature)

### ESP32

- [ESP32 - Bluetooth Analog Gauge](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-analog-gauge)
- [ESP32 - Bluetooth Chat](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-chat)
- [ESP32 - Bluetooth Digital Pins](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-digital-pins)
- [ESP32 - Bluetooth Joystick](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-joystick)
- [ESP32 - Bluetooth Monitor](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-monitor)
- [ESP32 - Bluetooth Multiple Apps](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-multiple-apps)
- [ESP32 - Bluetooth Plotter](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-plotter)
- [ESP32 - Bluetooth Rotator](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-rotator)
- [ESP32 - Bluetooth Slider](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-slider)
- [ESP32 - Bluetooth Table](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-table)
- [ESP32 - Bluetooth Temperature](https://esp32io.com/tutorials/esp32-diyables-bluetooth-app-temperature)


### Arduino MKR WiFi 1010

- [Arduino MKR WiFi 1010 - Bluetooth Analog Gauge](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-analog-gauge)
- [Arduino MKR WiFi 1010 - Bluetooth Chat](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-chat)
- [Arduino MKR WiFi 1010 - Bluetooth Digital Pins](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-digital-pins)
- [Arduino MKR WiFi 1010 - Bluetooth Joystick](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-joystick)
- [Arduino MKR WiFi 1010 - Bluetooth Monitor](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-monitor)
- [Arduino MKR WiFi 1010 - Bluetooth Multiple Apps](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-multiple-apps)
- [Arduino MKR WiFi 1010 - Bluetooth Plotter](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-plotter)
- [Arduino MKR WiFi 1010 - Bluetooth Rotator](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-rotator)
- [Arduino MKR WiFi 1010 - Bluetooth Slider](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-slider)
- [Arduino MKR WiFi 1010 - Bluetooth Table](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-table)
- [Arduino MKR WiFi 1010 - Bluetooth Temperature](https://newbiely.com/tutorials/arduino-mkr/arduino-mkr-wifi-1010-diyables-bluetooth-app-temperature)



### Arduino Nano 33 IoT

- [Arduino Nano 33 IoT - Bluetooth Analog Gauge](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-analog-gauge)
- [Arduino Nano 33 IoT - Bluetooth Chat](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-chat)
- [Arduino Nano 33 IoT - Bluetooth Digital Pins](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-digital-pins)
- [Arduino Nano 33 IoT - Bluetooth Joystick](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-joystick)
- [Arduino Nano 33 IoT - Bluetooth Monitor](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-monitor)
- [Arduino Nano 33 IoT - Bluetooth Multiple Apps](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-multiple-apps)
- [Arduino Nano 33 IoT - Bluetooth Plotter](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-plotter)
- [Arduino Nano 33 IoT - Bluetooth Rotator](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-rotator)
- [Arduino Nano 33 IoT - Bluetooth Slider](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-slider)
- [Arduino Nano 33 IoT - Bluetooth Table](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-table)
- [Arduino Nano 33 IoT - Bluetooth Temperature](https://newbiely.com/tutorials/arduino-nano-iot/arduino-nano-33-iot-diyables-bluetooth-app-temperature)


