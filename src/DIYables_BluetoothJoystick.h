#ifndef DIYABLES_BLUETOOTH_JOYSTICK_H
#define DIYABLES_BLUETOOTH_JOYSTICK_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Joystick app - manages joystick interface
 * Handles joystick coordinate messages in format: "JOYSTICK:x,y"
 */
class DIYables_BluetoothJoystick : public DIYables_BluetoothAppBase {
public:
    // Callback type definitions
    typedef void (*JoystickCallback)(int x, int y);
    typedef void (*JoystickConfigCallback)();

private:
    JoystickCallback joystickCallback;
    JoystickConfigCallback configCallback;
    bool autoReturn;
    float sensitivity;

public:
    DIYables_BluetoothJoystick(bool autoReturn = false, float sensitivity = 1.0);
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Configuration methods
    void setAutoReturn(bool enabled);
    bool getAutoReturn() const;
    void setSensitivity(float value);
    float getSensitivity() const;
    
    // Callback setters
    void onJoystickValue(JoystickCallback callback);
    void onGetConfig(JoystickConfigCallback callback);
    
    // Send methods
    void send(const String& message);
    void send(int x, int y);  // Convenience method for sending coordinates
};

#endif
