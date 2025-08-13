#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>

class TCPService : public ComService
{
private:
    int server_fd;
    int client_fd;
    struct sockaddr_in address;
    static constexpr int opt = 1;
    std::atomic<bool> end;
    std::thread worker_thread;

    void handle_connection(void);

public:
    ~TCPService()
    {
        if (server_fd != -1)
        {
            close(server_fd);
        }
        if (client_fd != -1)
        {
            close(client_fd);
        }
        end = true;
        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
    }
    TCPService();

    void run() override;
};

#endif
