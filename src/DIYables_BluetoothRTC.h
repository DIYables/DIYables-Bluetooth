#ifndef DIYABLES_BLUETOOTH_RTC_H
#define DIYABLES_BLUETOOTH_RTC_H

#include "DIYables_BluetoothAppBase.h"

/**
 * Bluetooth RTC app - manages real-time clock display and time synchronization
 * Message formats:
 * - "RTC:SYNC:timestamp" - Sync time with Unix timestamp
 * - "RTC:SYNC:Y,M,D,H,M,S" - Sync with date/time components
 * - "RTC:GET_TIME" - Request current time
 */
class DIYables_BluetoothRTC : public DIYables_BluetoothAppBase {
public:
    // Callback type definitions
    typedef void (*TimeSyncCallback)(unsigned long unixTimestamp);
    typedef void (*LocalTimeSyncCallback)(int year, int month, int day, int hour, int minute, int second);
    typedef void (*TimeRequestCallback)();

private:
    TimeSyncCallback timeSyncCallback;
    LocalTimeSyncCallback localTimeSyncCallback;
    TimeRequestCallback timeRequestCallback;

public:
    DIYables_BluetoothRTC();
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Callback setters
    void onTimeSync(TimeSyncCallback callback);
    void onLocalTimeSync(LocalTimeSyncCallback callback);
    void onTimeRequest(TimeRequestCallback callback);
    
    // Send methods
    void send(const String& message);
    void sendTime(unsigned long unixTimestamp);
    void sendTime(int year, int month, int day, int hour, int minute, int second);
};

#endif
