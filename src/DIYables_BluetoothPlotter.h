#ifndef DIYABLES_BLUETOOTH_PLOTTER_H
#define DIYABLES_BLUETOOTH_PLOTTER_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth Plotter app - manages real-time data plotting interface
 * Sends data in format: "PLOTTER:value1,value2,value3"
 */
class DIYables_BluetoothPlotter : public DIYables_BluetoothAppBase {
public:
    // Callback type definition
    typedef void (*PlotterDataCallback)();

private:
    PlotterDataCallback dataCallback;
    String plotTitle;
    String xAxisLabel;
    String yAxisLabel;
    float yAxisMin;
    float yAxisMax;
    bool autoScale;
    int maxSamples;
    String legendLabels[10]; // Support up to 10 data series
    int legendCount;

public:
    DIYables_BluetoothPlotter();
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Callback setter
    void onDataRequest(PlotterDataCallback callback);
    
    // Send methods for plotting data
    void send(float value);
    void send(float value1, float value2);
    void send(float value1, float value2, float value3);
    void send(float value1, float value2, float value3, float value4);
    void send(float values[], int count);
    void send(const String& dataLine);
    
    // Configuration methods
    void setPlotTitle(const String& title);
    void setAxisLabels(const String& xLabel, const String& yLabel);
    void setYAxisRange(float minY, float maxY);
    void enableAutoScale(bool enable = true);
    void setMaxSamples(int samples);
    void setLegendLabels(const String labels[], int count);
    void setLegendLabels(const String& label1);
    void setLegendLabels(const String& label1, const String& label2);
    void setLegendLabels(const String& label1, const String& label2, const String& label3);
    void setLegendLabels(const String& label1, const String& label2, const String& label3, const String& label4);
};

#endif
