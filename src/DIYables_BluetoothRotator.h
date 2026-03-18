#ifndef DIYABLES_BLUETOOTH_ROTATOR_H
#define DIYABLES_BLUETOOTH_ROTATOR_H

#include "DIYables_BluetoothAppBase.h"

// Rotator mode constants
#define ROTATOR_MODE_CONTINUOUS 0
#define ROTATOR_MODE_LIMITED 1

/**
 * Bluetooth Rotator app - manages rotatable disc/knob interface
 * Message format: "ROTATOR:angle" (0-360 degrees)
 * Supports continuous rotation or limited angle range
 */
class DIYables_BluetoothRotator : public DIYables_BluetoothAppBase {
public:
    // Callback type definitions
    typedef void (*RotatorCallback)(float angle);
    typedef void (*RotatorConfigCallback)();

private:
    RotatorCallback rotatorCallback;
    RotatorConfigCallback configCallback;
    int rotatorMode;
    float minAngle;
    float maxAngle;

public:
    DIYables_BluetoothRotator(int mode = ROTATOR_MODE_CONTINUOUS);
    DIYables_BluetoothRotator(int mode, float minAng, float maxAng);
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Configuration methods
    void setRotatorMode(int mode, float minAng = 0, float maxAng = 360);
    int getRotatorMode() const;
    float getMinAngle() const;
    float getMaxAngle() const;
    
    // Callback setters
    void onRotatorAngle(RotatorCallback callback);
    void onGetConfig(RotatorConfigCallback callback);
    
    // Send methods
    void send(const String& message);
    void send(float angle);  // Send angle value
};

#endif
