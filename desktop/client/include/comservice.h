#ifndef COMSERVICE_H
#define COMSERVICE_H

class COMService
{

protected:
    // Enum för meddelandetyper
    enum class MessageType
    {
        SPEED = 1,
        TEMPERATURE = 2,
        BATTERY = 3
    };

    // Struktur för datapaket
    struct DataPacket
    {
        MessageType messageType;
        int value;
    };

public:
    // Initiera kommunikation (öppna socket, port etc.)
    virtual bool connect() = 0;

    virtual bool sendData(const DataPacket &data) = 0;

    virtual DataPacket receiveData() = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() const = 0;
};

#endif