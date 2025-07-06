#include <cstdlib>
#include <thread>
#include <cstring>
#include "tcpservice.h"

TCPService::TCPService() : COMService(), end{false}
{
    /* ----- create server ------- */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket failed\n";
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        std::cerr << "Setsockopt failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(Settings::Server::PORT);

    if (bind((server_fd), (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* ----- connection thread init ------- */
    std::thread(&TCPService::handle_connection, this).detach();
}

void TCPService::handle_connection(void)
{

    while (!end)
    {
        if (listen(server_fd, 3) < 0)
        {
            std::cerr << "Listen failed\n";
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Server is listening on port " << Settings::Server::PORT << std::endl;

        socklen_t addrlen = sizeof(address);
        if ((client_fd =
                 accept(server_fd, (struct sockaddr *)&address,
                        &addrlen)) < 0)
        {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "Connection accepted\n";

        run();
        close(client_fd);
    }
}

void TCPService::run()
{
    while (!end)
    {
        uint8_t temp_buffer[sizeof(buffer)];

        {
            std::lock_guard<std::mutex> lock(mutex);
            std::memcpy(temp_buffer, buffer, sizeof(buffer));
        }

        if (send(client_fd, temp_buffer, sizeof(temp_buffer), 0) == -1)
        {
            std::cerr << "Failed to send buffer\n";
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
