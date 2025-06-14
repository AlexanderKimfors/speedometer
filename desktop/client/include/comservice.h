#ifndef COMSERVICE_H
#define COMSERVICE_H

#include <stdint.h>

class COMService
{

protected:
    // Enum för meddelandetyper
    enum class MessageType : int32_t
    {
        SPEED = 1,
        TEMPERATURE = 2,
        BATTERY = 3
    };

    // Struktur för datapaket
    struct DataPacket
    {
        MessageType messageType;
        int data;
    };

public:
    // Initiera kommunikation (öppna socket, port etc.)
    virtual bool connect() = 0;

    virtual bool sendData(const DataPacket &data) = 0;

    virtual bool receiveData(DataPacket &data) = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() const = 0;
};

#endif