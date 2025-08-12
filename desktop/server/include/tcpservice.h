#ifndef TCPCOM_H
#define TCPCOM_H
#include "comservice.h"
#include <atomic>
#include <thread>
#include <string>

class TCPService : public ComService
{
public:
    TCPService();
    ~TCPService();

    void run(void) override;               // Starts the server in a thread
    void stop();                           // Signals to stop the server
    void send(const std::string &message); // Send message to connected client

private:
    void serverLoop(); // The actual server loop

    std::atomic<bool> running;
    std::thread serverThread;
    std::string tempBuffer;

    int sockfd = -1;
    int connfd = -1;
};

#endif // TCPCOM_H