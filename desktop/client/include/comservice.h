#ifndef COMSERVICE_H
#define COMSERVICE_H

class COMService
{
public:
    // Initiera kommunikation (öppna socket, port etc.)
    virtual bool connect() = 0;

    virtual bool sendData(const int &data) = 0;

    virtual int receiveData() = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() const = 0;
};

#endif