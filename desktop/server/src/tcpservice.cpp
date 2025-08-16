#include <cstdlib>
#include <cstring>
#include "tcpservice.h"
#include <iostream>

TCPService::TCPService() : ComService()
{
    static constexpr int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    bool server_status{true};

    if (0 > server_fd)
    {
        std::cout << "Failed to create server socket" << std::endl;
        server_status = false;
    }
    else
    {
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)))
        {
            std::cerr << "Setsockopt failed\n";
            close(server_fd);
            server_fd = -1;
            exit(EXIT_FAILURE);
        }

        sockaddr_in server_addr{};

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(settings::Server::PORT);

        if (0 > bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)))
        {
            std::cout << "Failed to bind the server to the file descriptor" << std::endl;
            server_status = false;
        }
        else
        {
            if (0 > listen(server_fd, 3))
            {
                std::cout << "Failed to set the server in listen mode" << std::endl;
                server_status = false;
            }
            else
            {
                std::cout << "The server in now up and running and listening for clients" << std::endl;
            }
        }
    }

    if (server_status)
    {
        worker_thread = std::thread(&TCPService::run, this);
    }
}

void TCPService::run()
{
    int connection_fd;
    while (!end)
    {
        sockaddr_in client_addr{};
        socklen_t len{sizeof(client_addr)};

        connection_fd = accept(server_fd, (sockaddr *)&client_addr, &len);

        if (0 > connection_fd)
        {
            std::cout << "Failed to accepted the client" << std::endl;
            status = false;
        }
        else
        {
            std::cout << "Connection established" << std::endl;
            status = true;

            uint8_t temp_buffer[BUFFLEN]{};

            while (!end)
            {
                {
                    std::scoped_lock lock(buffer_mtx);
                    std::memcpy(temp_buffer, buffer, BUFFLEN);
                }
                if (BUFFLEN == send(connection_fd, temp_buffer, BUFFLEN, 0))
                {
                    status = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
                else
                {
                    std::cout << "Failed to send the buffer to the client" << std::endl;
                    status = false;
                    break;
                }
            }
            shutdown(connection_fd, SHUT_RDWR);
            close(connection_fd);
        }
    }
}
