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
    int connection_fd;
    std::atomic<bool> end{false};
    std::thread worker_thread;

    void run() override;

public:
    TCPService();

    ~TCPService()
    {
        end = true;

        shutdown(server_fd, SHUT_RDWR);
        shutdown(connection_fd, SHUT_RDWR);
        close(server_fd);
        close(connection_fd);

        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
    }
};

#if 0
class TCPService : public ComService
{
private:
    int server_fd;
    int client_fd;
    struct sockaddr_in address;
    static constexpr int opt = 1;
    std::atomic<bool> end;
    std::mutex mutex;
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

#if 0 // farochs kod
class TCPService : public ComService
{
private:
    int socket_fd;
    std::atomic<bool> end{false};
    std::thread worker_thread{&TCPService::run, this};

    void run() override;

public:
    ~TCPService()
    {
        end = true;
        shutdown(socket_fd, SHUT_RDWR);
        if (server_fd != -1)
        {
            close(server_fd);
        }
        if (client_fd != -1)
        {
            close(client_fd);
        }
        if (worker_thread.joinable())
        {
            worker_thread.join();
        }
    }
    TCPService() = default;
};
#endif

#endif
