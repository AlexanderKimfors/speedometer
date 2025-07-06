#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "comservice.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <atomic>
#include <thread>

class TCPService : public COMService
{
private:
    int client_fd;
    struct sockaddr_in server_address;
    std::atomic<bool> connected;
    std::atomic<bool> end;
    std::thread worker_thread;

public:
    TCPService();
    ~TCPService()
    {
        end = true;
        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
    }

    bool connected_to_server();
    void run() override;
    bool get_connection_state() override;
};

#endif

/*
SINGELTON


    // Singelton, no copy
    TCPService(const TCPService &) = delete;
    TCPService &operator=(const TCPService &) = delete;

    // Singelton, no move
    TCPService(TCPService &&) = delete;
    TCPService &operator=(TCPService &&) = delete;

    // Singleton-access
    static TCPService &getInstance()
    {
        static TCPService instance;
        return instance;
    }
*/