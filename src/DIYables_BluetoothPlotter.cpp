#include "DIYables_BluetoothPlotter.h"

DIYables_BluetoothPlotter::DIYables_BluetoothPlotter() 
    : DIYables_BluetoothAppBase(),
      dataCallback(nullptr),
      plotTitle(""),
      xAxisLabel(""),
      yAxisLabel(""),
      yAxisMin(0),
      yAxisMax(0),
      autoScale(true),
      maxSamples(100),
      legendCount(0) {
}

void DIYables_BluetoothPlotter::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for plotter
    if (!msg.startsWith("PLOTTER:")) {
        return;
    }
    
    // Handle request for plotter data
    if (msg == "PLOTTER:GET_DATA") {
        if (dataCallback) {
            dataCallback();
        }
        return;
    }
    
    // Handle request for plotter configuration
    if (msg == "PLOTTER:GET_CONFIG") {
        String response = "PLOTTER_CONFIG:{";
        response += "\"title\":\"" + plotTitle + "\",";
        response += "\"xLabel\":\"" + xAxisLabel + "\",";
        response += "\"yLabel\":\"" + yAxisLabel + "\",";
        response += "\"yMin\":" + String(yAxisMin, 2) + ",";
        response += "\"yMax\":" + String(yAxisMax, 2) + ",";
        response += "\"autoScale\":" + String(autoScale ? "true" : "false") + ",";
        response += "\"maxSamples\":" + String(maxSamples) + ",";
        response += "\"legendLabels\":[";
        for (int i = 0; i < legendCount; i++) {
            if (i > 0) response += ",";
            response += "\"" + legendLabels[i] + "\"";
        }
        response += "]";
        response += "}";
        DIYables_BluetoothAppBase::send(response);
        return;
    }
}

void DIYables_BluetoothPlotter::onDataRequest(PlotterDataCallback callback) {
    dataCallback = callback;
}

void DIYables_BluetoothPlotter::send(float value) {
    String message = "PLOTTER:";
    message += String(value, 2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::send(float value1, float value2) {
    String message = "PLOTTER:";
    message += String(value1, 2);
    message += ",";
    message += String(value2, 2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::send(float value1, float value2, float value3) {
    String message = "PLOTTER:";
    message += String(value1, 2);
    message += ",";
    message += String(value2, 2);
    message += ",";
    message += String(value3, 2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::send(float value1, float value2, float value3, float value4) {
    String message = "PLOTTER:";
    message += String(value1, 2);
    message += ",";
    message += String(value2, 2);
    message += ",";
    message += String(value3, 2);
    message += ",";
    message += String(value4, 2);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::send(float values[], int count) {
    String message = "PLOTTER:";
    for (int i = 0; i < count; i++) {
        if (i > 0) message += ",";
        message += String(values[i], 2);
    }
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::send(const String& dataLine) {
    String message = "PLOTTER:";
    message += dataLine;
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothPlotter::setPlotTitle(const String& title) {
    plotTitle = title;
}

void DIYables_BluetoothPlotter::setAxisLabels(const String& xLabel, const String& yLabel) {
    xAxisLabel = xLabel;
    yAxisLabel = yLabel;
}

void DIYables_BluetoothPlotter::setYAxisRange(float minY, float maxY) {
    yAxisMin = minY;
    yAxisMax = maxY;
    autoScale = false;
}

void DIYables_BluetoothPlotter::enableAutoScale(bool enable) {
    autoScale = enable;
}

void DIYables_BluetoothPlotter::setMaxSamples(int samples) {
    maxSamples = samples;
}

void DIYables_BluetoothPlotter::setLegendLabels(const String labels[], int count) {
    legendCount = (count > 10) ? 10 : count;
    for (int i = 0; i < legendCount; i++) {
        legendLabels[i] = labels[i];
    }
}

void DIYables_BluetoothPlotter::setLegendLabels(const String& label1) {
    legendCount = 1;
    legendLabels[0] = label1;
}

void DIYables_BluetoothPlotter::setLegendLabels(const String& label1, const String& label2) {
    legendCount = 2;
    legendLabels[0] = label1;
    legendLabels[1] = label2;
}

void DIYables_BluetoothPlotter::setLegendLabels(const String& label1, const String& label2, const String& label3) {
    legendCount = 3;
    legendLabels[0] = label1;
    legendLabels[1] = label2;
    legendLabels[2] = label3;
}

void DIYables_BluetoothPlotter::setLegendLabels(const String& label1, const String& label2, const String& label3, const String& label4) {
    legendCount = 4;
    legendLabels[0] = label1;
    legendLabels[1] = label2;
    legendLabels[2] = label3;
    legendLabels[3] = label4;
}
