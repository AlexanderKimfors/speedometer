#include "tcpservice.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

tcpservice::tcpservice(void)
{
    sockfd = -1;
    (void)std::memset(&servaddr, 0, sizeof(servaddr));
}

int32_t tcpservice::init(const char *ip_addr, uint16_t port)
{
    int32_t status = -1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd >= 0)
    {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip_addr, &servaddr.sin_addr) > 0)
        {
            status = 0;
        }
        else
        {
            (void)close(sockfd);
            sockfd = -1;
        }
    }

    return status;
}

int32_t tcpservice::connectToServer(void)
{
    int32_t status = -1;
    if (sockfd >= 0)
    {
        if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&servaddr), sizeof(servaddr)) == 0)
        {
            status = 0;
        }
    }
    return status;
}

void tcpservice::closeConnection(void)
{
    if (sockfd >= 0)
    {
        (void)shutdown(sockfd, SHUT_RDWR);
        (void)close(sockfd);
        sockfd = -1;
    }
}

int32_t tcpservice::requestIntValue(const char *command)
{
    int32_t value = -1;
    char buffer[32];
    ssize_t bytes_sent = 0;
    ssize_t bytes_read = 0;

    (void)std::memset(buffer, 0, sizeof(buffer));

    if ((sockfd >= 0) && (command != nullptr))
    {
        bytes_sent = write(sockfd, command, std::strlen(command));
        if (bytes_sent == static_cast<ssize_t>(std::strlen(command)))
        {
            bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                value = std::atoi(buffer);
            }
        }
    }

    return value;
}

bool tcpservice::requestBoolValue(const char *command)
{
    bool result = false;
    char buffer[8];
    ssize_t bytes_sent = 0;
    ssize_t bytes_read = 0;

    (void)std::memset(buffer, 0, sizeof(buffer));

    if ((sockfd >= 0) && (command != nullptr))
    {
        bytes_sent = write(sockfd, command, std::strlen(command));
        if (bytes_sent == static_cast<ssize_t>(std::strlen(command)))
        {
            bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                if (std::strncmp(buffer, "1", 1) == 0)
                {
                    result = true;
                }
            }
        }
    }

    return result;
}