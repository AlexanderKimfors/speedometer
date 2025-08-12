#include "setting.h"
#include "tcpservice.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


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
    servaddr.sin_port = htons(12345); // Hardcoded port here
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
    std::cout << "Waiting for client...\n";
    connfd = accept(sockfd, (sockaddr *)&cli, &len);
    if (connfd < 0)
    {
        std::cerr << "Accept failed.\n";
        return;
    }

    std::cout << "Client connected.\n";

    while (running)
    {
        std::string localCopy;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!tempBuffer.empty())
            {
                localCopy = tempBuffer;
                tempBuffer.clear();
            }
        }

        if (!localCopy.empty())
        {
            ssize_t sent = write(connfd, localCopy.c_str(), localCopy.size());
            if (sent != static_cast<ssize_t>(localCopy.size()))
            {
                std::cerr << "Failed to send full message.\n";
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Shutting down TCP server...\n";
}
