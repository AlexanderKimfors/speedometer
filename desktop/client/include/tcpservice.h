#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <cstdint>
#include <netinet/in.h>

class TcpClientService
{
public:
    TcpClientService(void);
    int32_t init(const char *ip_addr, uint16_t port);
    int32_t connectToServer(void);
    void closeConnection(void);

private:
    int32_t requestIntValue(const char *command);
    bool requestBoolValue(const char *command);

    int sockfd;
    struct sockaddr_in servaddr;
};

#endif