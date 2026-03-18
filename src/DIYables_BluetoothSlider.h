#ifndef DIYABLES_BLUETOOTH_SLIDER_H
#define DIYABLES_BLUETOOTH_SLIDER_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Slider app - manages slider control interface
 * Handles slider value messages in format: "SLIDER:value1,value2"
 * Values range from 0 to 100
 */
class DIYables_BluetoothSlider : public DIYables_BluetoothAppBase {
public:
    // Callback type definitions
    typedef void (*SliderCallback)(int slider1, int slider2);
    typedef void (*SliderConfigCallback)();

private:
    SliderCallback sliderCallback;
    SliderConfigCallback configCallback;
    int minValue;
    int maxValue;
    int stepValue;

public:
    DIYables_BluetoothSlider(int min = 0, int max = 100, int step = 1);
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Configuration methods
    void setRange(int min, int max);
    void setStep(int step);
    int getMin() const;
    int getMax() const;
    int getStep() const;
    
    // Callback setters
    void onSliderValue(SliderCallback callback);
    void onGetConfig(SliderConfigCallback callback);
    
    // Send methods
    void send(const String& message);
    void send(int slider1, int slider2);  // Two sliders
    void send(int value);  // Single slider
};

#endif
