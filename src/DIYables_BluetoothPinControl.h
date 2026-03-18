#ifndef DIYABLES_BLUETOOTH_PIN_CONTROL_H
#define DIYABLES_BLUETOOTH_PIN_CONTROL_H

#include "DIYables_BluetoothAppBase.h"

// Pin mode constants
#define BT_PIN_OUTPUT 0
#define BT_PIN_INPUT 1

/**
 * Bluetooth Pin Control/Monitor app - manages digital and analog pin control and monitoring
 * Message formats:
 * - PIN:pin,state (e.g., "PIN:13,1" to set pin 13 HIGH)
 * - MODE:pin,mode (e.g., "MODE:13,0" to set pin 13 as OUTPUT)
 * - READ:pin (e.g., "READ:13" to read pin 13 state)
 */
class DIYables_BluetoothPinControl : public DIYables_BluetoothAppBase {
public:
    // Callback type definitions
    typedef void (*PinWriteCallback)(int pin, int state);
    typedef int (*PinReadCallback)(int pin);
    typedef void (*PinModeCallback)(int pin, int mode);

private:
    PinWriteCallback pinWriteCallback;
    PinReadCallback pinReadCallback;
    PinModeCallback pinModeCallback;
    
    bool enabledPins[16];    // Track which pins are enabled (pins 0-15)
    int pinModes[16];        // Track pin modes (OUTPUT, INPUT)
    String pinNames[16];     // Track pin names for display in app
    
    void handlePinCommand(const String& message);
    void handlePinModeCommand(const String& message);
    void handleReadCommand(const String& message);
    String buildPinConfigJson();
    String buildPinStatesJson();

public:
    DIYables_BluetoothPinControl();
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Pin management methods
    void enablePin(int pin, int mode = BT_PIN_OUTPUT, const String& name = "");
    void disablePin(int pin);
    void enableAllPins(int mode = BT_PIN_OUTPUT);
    void disableAllPins();
    bool isPinEnabled(int pin);
    int getPinMode(int pin);
    void setPinMode(int pin, int mode);
    int getEnabledPinCount();
    
    // Callback setters
    void onPinWrite(PinWriteCallback callback);
    void onPinRead(PinReadCallback callback);
    void onPinModeChange(PinModeCallback callback);
    
    // Send methods
    void send(const String& message);
    void updatePinState(int pin, int state);  // For real-time input monitoring
};

#endif
