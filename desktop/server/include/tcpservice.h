#ifndef TCPCOM_H
#define TCPCOM_H
#include "comservice.h"
#include <netinet/in.h>

class TCPService : public COMService
{
public:
    TCPService(int port = 12345);
    ~TCPService();

    void run(); // Starts the server loop

private:
    int sockfd_;
    int connfd_;
    int port_;
    sockaddr_in servaddr_;
};

#endif // TCPCOM_H