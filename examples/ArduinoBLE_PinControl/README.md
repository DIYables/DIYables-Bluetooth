# DIYables Bluetooth Digital Pins Example

This example demonstrates how to use the DIYables Bluetooth Digital Pins library with the new protocol.

## Features

- **Automatic Configuration**: App requests pin configuration from Arduino
- **Custom Pin Names**: Assign friendly names to pins for display in the app
- **Real-time Monitoring**: Automatically sends updates when input pins change
- **Two-way Communication**: Control outputs and monitor inputs

## Protocol

### Config Request
```
App → Arduino: "PINS:GET_CONFIG"
Arduino → App: "PINS_CONFIG:[{pin:13,name:"Built-in LED",type:"output",value:0}, ...]"
```

### Pin Control
```
App → Arduino: "PINS:13,1" (turn on pin 13)
Arduino → App: "PINS:13,1" (confirmation)
```

### Input Updates
```
Arduino → App: "PINS:7,1" (when input pin 7 changes to HIGH)
```

## Customization

### Add Custom Pin Names
```cpp
bluetoothPins.enablePin(13, BT_PIN_OUTPUT, "Built-in LED");
bluetoothPins.enablePin(12, BT_PIN_OUTPUT, "Motor Control");
bluetoothPins.enablePin(7, BT_PIN_INPUT, "Button 1");
```

### Using Default Names
```cpp
bluetoothPins.enablePin(13, BT_PIN_OUTPUT);  // Will show as "Pin 13"
```

## Hardware Setup

- Arduino Nano 33 BLE or similar BLE-capable board
- Connect LEDs/relays to output pins (12, 11, 13)
- Connect buttons/sensors to input pins (7, 6) with INPUT_PULLUP

## Usage

1. Upload this sketch to your Arduino
2. Open the DIYables Bluetooth App
3. Connect to "Arduino_Pins"
4. Navigate to Digital Pins screen
5. App automatically loads pin configuration from Arduino
6. Toggle output pins on/off
7. Input pins update in real-time when states change

## Notes

- Pin names are customizable and will appear in the app
- Input pins use INPUT_PULLUP mode (LOW when button pressed)
- Output pins default to LOW on startup
- The library handles all protocol communication automatically
