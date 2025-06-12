#include "setting.h"
#include "tcpservice.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cctype>
#include <arpa/inet.h>
#include <sys/socket.h>

constexpr int MAX_LEN{32};

TCPService::TCPService(int port)
    : sockfd_(-1), connfd_(-1), port_(port)
{
    std::memset(&servaddr_, 0, sizeof(servaddr_));
}

TCPService::~TCPService()
{
    if (connfd_ != -1)
    {
        shutdown(connfd_, SHUT_RDWR);
        close(connfd_);
    }

    if (sockfd_ != -1)
    {
        shutdown(sockfd_, SHUT_RDWR);
        close(sockfd_);
    }
}

void TCPService::run()
{
    sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd_ == -1)
    {
        std::cerr << "Failed to create the socket..." << std::endl;
        return;
    }

    servaddr_.sin_family = AF_INET;
    servaddr_.sin_port = htons(port_);
    servaddr_.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd_, (sockaddr *)&servaddr_, sizeof(servaddr_)) != 0)
    {
        std::cerr << "Failed to bind servaddr to the socket..." << std::endl;
        return;
    }

    if (listen(sockfd_, 1) != 0)
    {
        std::cerr << "Failed to listen to the port..." << std::endl;
        return;
    }

    sockaddr_in cli{0};
    socklen_t len = sizeof(cli);

    connfd_ = accept(sockfd_, (sockaddr *)&cli, &len);
    if (connfd_ < 0)
    {
        std::cerr << "Failed to accept the connection..." << std::endl;
        return;
    }

    std::cout << "Server accepted the client..." << std::endl;

    while (true)
    {
        char buffer[MAX_LEN + 1]{0};

        ssize_t n = read(connfd_, buffer, MAX_LEN);
        if (n <= 0)
        {
            std::cerr << "Failed to read or client disconnected." << std::endl;
            break;
        }

        for (size_t i = 0; i < strlen(buffer); ++i)
        {
            buffer[i] = toupper(buffer[i]);
        }

        if (write(connfd_, buffer, strlen(buffer)) != static_cast<ssize_t>(strlen(buffer)))
        {
            std::cerr << "Failed to write ..." << std::endl;
            break;
        }
    }
}
