#include "setting.h"
#include "tcpservice.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr int PORT = 12345;
constexpr int MAX_LEN = 1024;

TCPService::TCPService() : running(false) {}

TCPService::~TCPService()
{
    stop();
}

void TCPService::run()
{
    if (running)
        return;

    running = true;
    serverThread = std::thread(&TCPService::serverLoop, this);
}

void TCPService::stop()
{
    running = false;

    if (connfd != -1)
    {
        shutdown(connfd, SHUT_RDWR);
        close(connfd);
        connfd = -1;
    }

    if (sockfd != -1)
    {
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
        sockfd = -1;
    }

    if (serverThread.joinable())
        serverThread.join();
}

void TCPService::serverLoop()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Socket creation failed.\n";
        return;
    }

    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        std::cerr << "Bind failed.\n";
        close(sockfd);
        sockfd = -1;
        return;
    }

    if (listen(sockfd, 1) != 0)
    {
        std::cerr << "Listen failed.\n";
        close(sockfd);
        sockfd = -1;
        return;
    }

    sockaddr_in cli{};
    socklen_t len = sizeof(cli);

    std::cout << "Waiting for client connection...\n";
    connfd = accept(sockfd, (sockaddr *)&cli, &len);
    if (connfd < 0)
    {
        std::cerr << "Accept failed.\n";
        return;
    }

    std::cout << "Client connected.\n";

    // Stay alive while server is running
    while (running)
    {
        // Wait or idle until something is sent via send()
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Server shutting down.\n";
}

void TCPService::send(const std::string &message)
{
    if (connfd == -1 || !running)
        return;

    char tempBuffer[MAX_LEN] = {0};
    std::size_t len = std::min(message.size(), static_cast<std::size_t>(MAX_LEN - 1));
    std::memcpy(tempBuffer, message.c_str(), len);

    ssize_t bytesSent = write(connfd, tempBuffer, len);
    if (bytesSent != (ssize_t)len)
    {
        std::cerr << "Failed to send message to client.\n";
    }
}