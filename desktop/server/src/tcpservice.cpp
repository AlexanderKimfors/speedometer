#include <cstdlib>
#include <cstring>
#include "tcpservice.h"

#if 0
TCPService::TCPService() : ComService(), end{false}
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
        server_fd = -1;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(settings::Server::PORT);

    if (bind((server_fd), (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
        server_fd = -1;
        exit(EXIT_FAILURE);
    }

    /* ----- connection thread init ------- */
    worker_thread = std::thread(&TCPService::handle_connection, this);
}

void TCPService::handle_connection(void)
{
    while (!end)
    {

        if (listen(server_fd, 3) < 0)
        {
            std::cerr << "Listen failed\n";
            close(server_fd);
            server_fd = -1;
            exit(EXIT_FAILURE);
        }
        std::cout << "Server is listening on port " << settings::Server::PORT << std::endl;

        socklen_t addrlen = sizeof(address);
        if ((client_fd =
                 accept(server_fd, (struct sockaddr *)&address,
                        &addrlen)) < 0)
        {
            std::cerr << "Accept failed\n";
            if (server_fd != -1)
            {
                close(server_fd);
                server_fd = -1;
            }
            exit(EXIT_FAILURE);
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
            std::scoped_lock lock(buffer_mtx);
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

#endif

void TCPService::run()
{
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(settings::Server::PORT);
    server_addr.sin_addr.s_addr = inet_addr(settings::Server::IP_ADRESS);

    while (!end)
    {
        socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    }
}