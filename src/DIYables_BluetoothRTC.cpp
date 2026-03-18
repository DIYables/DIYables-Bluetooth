#include "DIYables_BluetoothRTC.h"

DIYables_BluetoothRTC::DIYables_BluetoothRTC() 
    : DIYables_BluetoothAppBase(),
      timeSyncCallback(nullptr),
      localTimeSyncCallback(nullptr),
      timeRequestCallback(nullptr) {
}

void DIYables_BluetoothRTC::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for RTC (with RTC: prefix)
    if (!msg.startsWith("RTC:")) {
        return;
    }
    
    // Handle time synchronization with Unix timestamp
    if (msg.startsWith("RTC:SYNC:")) {
        String timeData = msg.substring(9);  // Skip "RTC:SYNC:"
        
        // Check if it's a Unix timestamp (single number) or date/time components
        int commaCount = 0;
        for (int i = 0; i < timeData.length(); i++) {
            if (timeData.charAt(i) == ',') commaCount++;
        }
        
        if (commaCount == 0) {
            // Unix timestamp format
            unsigned long timestamp = timeData.toInt();
            if (timeSyncCallback) {
                timeSyncCallback(timestamp);
            }
        } else if (commaCount == 5) {
            // Date/time components format: Y,M,D,H,M,S
            int year, month, day, hour, minute, second;
            int pos = 0;
            int nextPos;
            
            nextPos = timeData.indexOf(',', pos);
            year = timeData.substring(pos, nextPos).toInt();
            pos = nextPos + 1;
            
            nextPos = timeData.indexOf(',', pos);
            month = timeData.substring(pos, nextPos).toInt();
            pos = nextPos + 1;
            
            nextPos = timeData.indexOf(',', pos);
            day = timeData.substring(pos, nextPos).toInt();
            pos = nextPos + 1;
            
            nextPos = timeData.indexOf(',', pos);
            hour = timeData.substring(pos, nextPos).toInt();
            pos = nextPos + 1;
            
            nextPos = timeData.indexOf(',', pos);
            minute = timeData.substring(pos, nextPos).toInt();
            pos = nextPos + 1;
            
            second = timeData.substring(pos).toInt();
            
            if (localTimeSyncCallback) {
                localTimeSyncCallback(year, month, day, hour, minute, second);
            }
        }
        return;
    }
    
    // Handle request for current time
    if (msg == "RTC:GET_TIME") {
        if (timeRequestCallback) {
            timeRequestCallback();
        }
        return;
    }
}

void DIYables_BluetoothRTC::onTimeSync(TimeSyncCallback callback) {
    timeSyncCallback = callback;
}

void DIYables_BluetoothRTC::onLocalTimeSync(LocalTimeSyncCallback callback) {
    localTimeSyncCallback = callback;
}

void DIYables_BluetoothRTC::onTimeRequest(TimeRequestCallback callback) {
    timeRequestCallback = callback;
}

void DIYables_BluetoothRTC::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothRTC::sendTime(unsigned long unixTimestamp) {
    String message = "RTC_TIME:";
    message += String(unixTimestamp);
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothRTC::sendTime(int year, int month, int day, int hour, int minute, int second) {
    String message = "RTC_TIME:";
    message += String(year);
    message += ",";
    message += String(month);
    message += ",";
    message += String(day);
    message += ",";
    message += String(hour);
    message += ",";
    message += String(minute);
    message += ",";
    message += String(second);
    DIYables_BluetoothAppBase::send(message);
}
