#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <mutex>

class TCPService : public COMService
{
private:
    int server_fd;
    int client_fd;
    struct sockaddr_in address;
    static constexpr int opt = 1;
    std::atomic<bool> end;
    std::mutex mutex;

    void handle_connection(void);

public:
    ~TCPService()
    {
        close(server_fd);
        close(client_fd);
        end = true;
    }
    TCPService();

    void run() override;
};

#endif
