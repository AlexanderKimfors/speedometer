#ifndef TCPCOM_H
#define TCPCOM_H

#include "comservice.h"

constexpr int MESSAGE_LEN{8};

class TCPService : public COMService
{
private:
    int PORT;
    int sockfd;
    const char *SERVER_IP;

public:
    TCPService(int _PORT, char *_SERVER_IP) : PORT{_PORT}, SERVER_IP{_SERVER_IP} {}

    bool connect() override;

    bool sendData(const DataPacket &data) override;

    bool receiveData(DataPacket &data) override;

    void disconnect() override;

    bool isConnected() const override;
};

#endif