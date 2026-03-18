# DIYables Bluetooth Library

A comprehensive Bluetooth communication library for Arduino and ESP32 that communicates with the **DIYables Bluetooth STEM** mobile app on Android and iOS.

📲 **Download the app:**
- **Android**: [Google Play Store](https://play.google.com/store/apps/details?id=io.diyables.bluetoothstem)
- **iOS**: [App Store](#) *(coming soon)*

This library provides clean architecture with platform abstraction supporting both BLE (Bluetooth Low Energy) and Classic Bluetooth. Upload an Arduino sketch, connect via the mobile app, and start interacting with your hardware project — no mobile coding required.

## 🚀 Features

- **Multiple App Types**: Joystick, Plotter, Pin Control/Monitor, Chat, and Monitor
- **Clean Architecture**: SOLID principles with platform abstraction layer
- **Extensible Design**: Easy to add custom app types
- **BLE & Classic Support**: Works with ArduinoBLE, ESP32, and more
- **Ready Examples**: Complete working examples for each app type
- **Type-Safe API**: Clear interfaces with callback-based event handling

## 📱 Supported Hardware

- **Arduino Nano 33 BLE**: Native BLE support
- **ESP32**: Both BLE and Bluetooth Classic
- **Arduino Uno R4 WiFi**: BLE support
- **Any BLE-capable board**: Using ArduinoBLE library

## 📦 Installation

1. Download the library as ZIP
2. Open Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. Install dependency: `ArduinoBLE` library (for BLE boards)

## 🎯 Quick Start

```cpp
#include <DIYables_Bluetooth.h>

// BLE Configuration
DIYables_ArduinoBLE bluetooth("MyDevice", 
    "19B10000-E8F2-537E-4F6C-D104768A1214",
    "19B10001-E8F2-537E-4F6C-D104768A1214", 
    "19B10002-E8F2-537E-4F6C-D104768A1214");

DIYables_BluetoothServer bluetoothServer;
DIYables_BluetoothMonitor bluetoothMonitor;

void setup() {
    Serial.begin(9600);
    
    // Initialize server
    bluetoothServer.begin(&bluetooth);
    bluetoothServer.addApp(&bluetoothMonitor);
    
    // Set callbacks
    bluetoothServer.setOnConnected([]() {
        Serial.println("Connected!");
    });
    
    bluetoothMonitor.setMessageCallback([](const String& msg) {
        Serial.println(msg);
        bluetoothMonitor.send("Echo: " + msg);
    });
}

void loop() {
    bluetoothServer.loop();
}
```

## 📋 Available Apps

### 1. **Monitor** - Command & Message Handler
```cpp
DIYables_BluetoothMonitor monitor;
monitor.setMessageCallback([](const String& msg) {
    // Handle incoming messages
});
monitor.send("Hello");
```

### 2. **Joystick** - Virtual Joystick Control
```cpp
DIYables_BluetoothJoystick joystick(false, 5);  // autoReturn, sensitivity
joystick.onJoystickValue([](int x, int y) {
    // Handle joystick position (-100 to 100)
});
joystick.send(50, -30);  // Send position to app
```

### 3. **Plotter** - Real-time Data Visualization
```cpp
DIYables_BluetoothPlotter plotter;
plotter.setPlotTitle("Sensor Data");
plotter.setYAxisRange(-1.5, 1.5);
plotter.send(sensorValue1, sensorValue2, sensorValue3);
```

### 4. **Pin Control/Monitor** - Pin Control & Monitoring
```cpp
DIYables_BluetoothPinControl pins;
pins.enablePin(13, BT_PIN_OUTPUT);
pins.onPinWrite([](int pin, int state) {
    digitalWrite(pin, state);
});
pins.updatePinState(7, digitalRead(7));  // Send pin state
```

### 5. **Chat** - Text Messaging
```cpp
DIYables_BluetoothChat chat;
chat.onChatMessage([](const String& msg) {
    Serial.println(msg);
    chat.send("Echo: " + msg);
});
```

## 💡 Examples

Each app type includes a complete working example:

- **ArduinoBLE_Monitor**: Basic command handling and messaging
- **ArduinoBLE_Joystick**: Joystick position control
- **ArduinoBLE_Plotter**: Real-time data plotting (sine waves demo)
- **ArduinoBLE_DigitalPins**: Pin control/monitor with digital and analog pins
- **ArduinoBLE_Chat**: Two-way text communication

Find them in: `File → Examples → DIYables Bluetooth`

## 🏗️ Architecture

```
DIYables Bluetooth Library
├── IBluetooth (Interface)
│   └── Platform implementations
│       └── DIYables_ArduinoBLE
├── DIYables_BluetoothServer
│   └── Manages apps and routing
├── DIYables_BluetoothAppBase
│   └── Base class for all apps
└── App Implementations
    ├── DIYables_BluetoothMonitor
    ├── DIYables_BluetoothJoystick
    ├── DIYables_BluetoothPlotter
    ├── DIYables_BluetoothPinControl
    └── DIYables_BluetoothChat
```

### Key Design Principles

- **Platform Abstraction**: `IBluetooth` interface separates platform-specific code
- **Single Responsibility**: Each app handles one specific functionality
- **Open/Closed**: Easy to extend with new app types without modifying existing code
- **Dependency Inversion**: Apps depend on abstractions, not concrete implementations

## 🔧 Creating Custom Apps

```cpp
#include "DIYables_BluetoothAppBase.h"

class MyCustomApp : public DIYables_BluetoothAppBase {
public:
    MyCustomApp() : DIYables_BluetoothAppBase() {}
    
    void handleMessage(const String& message) override {
        if (message.startsWith("CUSTOM:")) {
            String data = message.substring(7);
            // Process your custom message
            send("CUSTOM_RESPONSE:" + data);
        }
    }
    
    void sendCustomData(int value) {
        send("CUSTOM:" + String(value));
    }
};
```

## 📡 Communication Protocol

Apps use prefixed message formats for routing:

- **Monitor**: `MONITOR:message`
- **Joystick**: `JOYSTICK:x,y` or `JOYSTICK:GET_CONFIG`
- **Plotter**: `PLOTTER:value1,value2,...` or `PLOTTER:GET_DATA`
- **Pin Control/Monitor**: `PIN:pin,state` or `MODE:pin,mode` or `READ:pin`
- **Chat**: `CHAT:message text`

The server automatically routes messages to the appropriate app based on the prefix.

## 🔄 Multiple Apps

You can run multiple apps simultaneously:

```cpp
DIYables_BluetoothMonitor monitor;
DIYables_BluetoothPlotter plotter;
DIYables_BluetoothJoystick joystick;

bluetoothServer.begin(&bluetooth);
bluetoothServer.addApp(&monitor);
bluetoothServer.addApp(&plotter);
bluetoothServer.addApp(&joystick);
```

Each app handles its own messages independently.

## ⚡ Platform-Specific Notes

### ArduinoBLE (Nano 33 BLE, Uno R4 WiFi)
```cpp
#include <platforms/DIYables_ArduinoBLE.h>

DIYables_ArduinoBLE bluetooth(
    "DeviceName",
    "ServiceUUID",
    "TxCharUUID", 
    "RxCharUUID"
);
```

### ESP32 (Future Support)
The architecture supports adding ESP32 Classic/BLE:
```cpp
// Coming soon: DIYables_ESP32Classic, DIYables_Esp32BLE
```

## 🆘 Troubleshooting

### Connection Issues
- Ensure `bluetoothServer.loop()` is called in main loop
- Check UUIDs match between Arduino and mobile app
- Verify Bluetooth is enabled on mobile device

### App Not Receiving Messages
- Confirm app is added to server: `bluetoothServer.addApp(&app)`
- Check message prefix matches app's expected format
- Use Serial.println() to debug incoming messages

### Compilation Errors
- Install `ArduinoBLE` library from Library Manager
- Verify board supports BLE (Nano 33 BLE, Uno R4 WiFi, etc.)
- Check `#include` paths match library structure

## 📚 API Reference

### IBluetooth Interface
- `bool begin()` - Initialize Bluetooth
- `void end()` - Shutdown Bluetooth
- `void loop()` - Handle communication (call frequently)
- `bool isConnected()` - Check connection status
- `bool send(const String& message)` - Send data
- `setOnConnected(ConnectionCallback)` - Connection event
- `setOnDisconnected(ConnectionCallback)` - Disconnection event
- `setOnMessage(MessageCallback)` - Message received event

### DIYables_BluetoothServer
- `void begin(IBluetooth* bluetooth)` - Initialize with platform
- `void loop()` - Handle routing
- `void addApp(DIYables_BluetoothAppBase* app)` - Register app
- `void removeApp(DIYables_BluetoothAppBase* app)` - Unregister app
- `int getAppCount()` - Get registered app count
- `setOnConnected(BluetoothEventCallback)` - Connection callback
- `setOnDisconnected(BluetoothEventCallback)` - Disconnection callback

### DIYables_BluetoothAppBase
- `virtual void handleMessage(const String& message)` - Override to handle messages
- `bool send(const String& message)` - Send message (protected)
- `bool isConnected()` - Check connection (protected)

## 📄 License

MIT License - see LICENSE file for details

## 🤝 Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create feature branch
3. Follow existing code style
4. Add examples for new features
5. Submit pull request

## 📞 Support

- **GitHub**: https://github.com/DIYables/DIYables-Bluetooth
- **Issues**: Report bugs and request features
- **Email**: support@diyables.com

## 📝 Changelog

### Version 1.0.0 (2025-11-24)
- Initial release
- Platform abstraction layer with IBluetooth interface
- ArduinoBLE platform implementation
- Five app types: Monitor, Joystick, Plotter, Pin Control/Monitor, Chat
- Complete examples for all app types
- Clean SOLID architecture
- Comprehensive documentation

## 🚀 Features

- **WebApp API Compatible**: Same method signatures as the WebSocket library for easy migration
- **Multi-platform Support**: ESP32 Bluetooth Classic and Arduino R4 WiFi BLE
- **Digital Pin Control**: Input/Output pin configuration and control
- **Analog Pin Reading**: Real-time analog sensor data
- **Monitor Functionality**: Real-time data streaming and command processing
- **JSON Communication**: Structured data exchange with mobile app

## 📱 Supported Hardware

- **ESP32**: Bluetooth Classic communication
- **Arduino R4 WiFi**: Built-in BLE support
- **Other Arduino**: With external ESP32 module

## 📦 Installation

1. Download the library as ZIP
2. Open Arduino IDE → Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. Install dependency: `ArduinoJson` library

## 🎯 Quick Start

```cpp
#include "DIYablesBluetoothApp.h"

DIYablesBluetoothApp btApp;

void setup() {
    Serial.begin(115200);
    
    // Initialize Bluetooth
    btApp.begin("My Arduino Device");
    
    // Set up pin callbacks
    btApp.onDigitalPinWrite(onDigitalPinWrite);
    
    // Set up monitor callback
    btApp.onMonitorMessage(onMonitorMessage);
    
    // Enable a digital output pin
    btApp.enableDigitalPin(2, "D2", "LED Control");
}

void loop() {
    btApp.loop(); // Handle Bluetooth communication
}

void onDigitalPinWrite(int pin, int state) {
    digitalWrite(pin, state);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" set to ");
    Serial.println(state);
}

void onMonitorMessage(const String& message) {
    Serial.print("Monitor command: ");
    Serial.println(message);
    
    if (message == "LED_ON") {
        digitalWrite(LED_BUILTIN, HIGH);
        btApp.sendToMonitor("status", "LED ON");
    }
}
```

## 📋 API Reference

### Initialization
- `void begin(const String& deviceName = "DIYables Device")`
- `void loop()` - Call this in main loop
- `bool getConnectionStatus()` - Check Bluetooth connection

### Pin Management
- `void enableDigitalPin(int pin, const String& pinName, const String& displayName, int type)`
- `void enableAnalogPin(int pin, const String& pinName, const String& displayName)`
- `void disablePin(int pin)`
- `void disableAllPins()`

### Callbacks
- `void onDigitalPinWrite(void (*callback)(int pin, int state))`
- `void onDigitalPinRead(int (*callback)(int pin))`
- `void onAnalogPinRead(int (*callback)(int pin))`
- `void onMonitorMessage(void (*callback)(const String& message))`

### Monitor Functions
- `void sendToMonitor(const String& message)`
- `void sendToMonitor(const String& key, const String& value)`
- `void sendToMonitor(const String& key, int value)`
- `void sendToMonitor(const String& key, float value)`

### Pin State Updates
- `void updatePinState(int pin, int state)`
- `void sendMessage(const String& message)`

## 💡 Examples

### Pin Control Examples
Located in `examples/DigitalPinControl/`:
- Control LEDs and read buttons
- Compatible with Pin Control/Monitor screen

### Monitor Examples
Located in `examples/Monitor*/`:
- **MonitorBasic**: Simple counter and command example
- **MonitorBLE**: Full-featured sensor monitoring (Bluetooth Classic)
- **MonitorBLE_Native**: Arduino R4 WiFi native BLE version

## 🔄 Migration from WebSocket Library

The API is designed for seamless migration:

**Before (WebSocket):**
```cpp
#include "DIYablesWebMonitor.h"
DIYablesWebMonitorPage webMonitorPage;

void setup() {
    webMonitorPage.onWebMonitorMessage(onMessage);
}

void onMessage(const String& message) {
    webMonitorPage.sendToWebMonitor("response", "OK");
}
```

**After (Bluetooth):**
```cpp
#include "DIYablesBluetoothApp.h"
DIYablesBluetoothApp btApp;

void setup() {
    btApp.begin("My Device");
    btApp.onMonitorMessage(onMessage);
}

void onMessage(const String& message) {
    btApp.sendToMonitor("response", "OK");
}
```

## 📱 Mobile App Compatibility

This library is compatible with the **DIYables Bluetooth Mobile App** screens:
- **Pin Control/Monitor**: Dynamic pin configuration and control for digital/analog pins
- **Monitor**: Real-time data streaming and command interface
- **Future screens**: Extensible JSON protocol for new features

## � Communication Protocol

The library uses JSON messages for structured communication:

```json
// Digital pin control
{"type": "DIGITAL_OUTPUT", "pin": "D2", "state": 1}

// Monitor data
{"type": "MONITOR", "key": "temperature", "value": 25.5}

// Commands
{"type": "MONITOR", "message": "LED_ON"}
```

## 🔧 Dependencies

- **ArduinoJson**: For JSON message parsing and generation
- **BluetoothSerial**: For ESP32 Bluetooth Classic (built-in)
- **ArduinoBLE**: For Arduino R4 WiFi BLE (install separately)

## � Troubleshooting

### Connection Issues
- Ensure Bluetooth is enabled on mobile device
- Check if device name appears in Bluetooth scan
- Verify correct library for your hardware (Classic vs BLE)

### Pin Control Issues
- Call `btApp.loop()` regularly in main loop
- Check pin numbers match Arduino board layout
- Verify pin is enabled before use

### Monitor Communication
- Check JSON message format in Serial Monitor
- Ensure callback functions are set before connection
- Verify mobile app is on correct screen

## ⚡ Hardware-Specific Notes

### ESP32
- Uses Bluetooth Classic (more compatible)
- No additional libraries needed
- Excellent range and stability

### Arduino R4 WiFi
- Two versions available: Classic (with ESP32) and native BLE
- BLE requires ArduinoBLE library
- Lower power consumption with BLE

## � License

This library is released under the MIT License. See LICENSE file for details.

## 🆘 Support

- **GitHub Issues**: https://github.com/DIYables/DIYables-Bluetooth-App/issues
- **Documentation**: https://diyables.io/bluetooth-app
- **Email**: support@diyables.com

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Submit a pull request

## � Changelog

### Version 1.0.0
- Initial release
- ESP32 Bluetooth Classic support
- Arduino R4 WiFi BLE support
- WebApp API compatibility
- Digital pin control
- Monitor functionality
- Comprehensive examples