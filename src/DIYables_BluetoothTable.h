#ifndef DIYABLES_BLUETOOTH_TABLE_H
#define DIYABLES_BLUETOOTH_TABLE_H

#include "DIYables_BluetoothAppBase.h"

// Maximum number of table rows
#define MAX_TABLE_ROWS 20

/**
 * Bluetooth Table app - manages a two-column table display with real-time updates
 * Message formats:
 * - "TABLE:ADD_ROW:attribute" - Add a row
 * - "TABLE:UPDATE:index,value" - Update row by index
 * - "TABLE:UPDATE:attribute,value" - Update row by attribute name
 * - "TABLE:CLEAR" - Clear all rows
 * - "TABLE:GET_DATA" - Request table data
 */
class DIYables_BluetoothTable : public DIYables_BluetoothAppBase {
public:
    // Callback type definition
    typedef void (*TableDataCallback)();

private:
    TableDataCallback dataCallback;
    String attributes[MAX_TABLE_ROWS];
    int rowCount;

public:
    DIYables_BluetoothTable();
    
    // Override from base class
    void handleBluetoothMessage(const char* message, uint16_t length) override;
    
    // Table management methods
    void addRow(const String& attribute);
    void clearTable();
    int getRowCount() const;
    String getAttribute(int index) const;
    
    // Callback setter
    void onDataRequest(TableDataCallback callback);
    
    // Send methods
    void send(const String& message);
    void sendValueUpdate(const String& attribute, const String& value);
    void sendValueUpdate(int index, const String& value);
    void sendTableStructure();  // Send current table structure
};

#endif
