#include "DIYables_BluetoothTable.h"

DIYables_BluetoothTable::DIYables_BluetoothTable() 
    : DIYables_BluetoothAppBase(),
      dataCallback(nullptr),
      rowCount(0) {
}

void DIYables_BluetoothTable::handleBluetoothMessage(const char* message, uint16_t length) {
    String msg(message);
    
    // Only handle messages intended for table (with TABLE: prefix)
    if (!msg.startsWith("TABLE:")) {
        return;
    }
    
    // Handle request for table configuration
    if (msg == "TABLE:GET_CONFIG") {
        sendTableStructure();
        return;
    }
    
    // Handle request for table data
    if (msg == "TABLE:GET_DATA") {
        if (dataCallback) {
            dataCallback();
        } else {
            sendTableStructure();
        }
        return;
    }
    
    // Handle clear command
    if (msg == "TABLE:CLEAR") {
        clearTable();
        return;
    }
}

void DIYables_BluetoothTable::addRow(const String& attribute) {
    if (rowCount < MAX_TABLE_ROWS) {
        attributes[rowCount] = attribute;
        rowCount++;
    }
}

void DIYables_BluetoothTable::clearTable() {
    rowCount = 0;
    for (int i = 0; i < MAX_TABLE_ROWS; i++) {
        attributes[i] = "";
    }
}

int DIYables_BluetoothTable::getRowCount() const {
    return rowCount;
}

String DIYables_BluetoothTable::getAttribute(int index) const {
    if (index >= 0 && index < rowCount) {
        return attributes[index];
    }
    return "";
}

void DIYables_BluetoothTable::onDataRequest(TableDataCallback callback) {
    dataCallback = callback;
}

void DIYables_BluetoothTable::send(const String& message) {
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothTable::sendValueUpdate(const String& attribute, const String& value) {
    String message = "VALUE_UPDATE:";
    message += attribute;
    message += ":";
    message += value;
    DIYables_BluetoothAppBase::send(message);
}

void DIYables_BluetoothTable::sendValueUpdate(int index, const String& value) {
    if (index >= 0 && index < rowCount) {
        sendValueUpdate(attributes[index], value);
    }
}

void DIYables_BluetoothTable::sendTableStructure() {
    String message = "TABLE_CONFIG:[";
    for (int i = 0; i < rowCount; i++) {
        if (i > 0) message += ",";
        message += "\"";
        message += attributes[i];
        message += "\"";
    }
    message += "]";
    DIYables_BluetoothAppBase::send(message);
}
