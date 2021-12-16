#include "server.h"
#include "helpers.h"
#include <chrono>
#include <cstdint>

std::ostream& operator<<(std::ostream& out, const std::vector<char>& s)
{
    for (const auto& c : s)
        out << c;
    return out;
}

Server::Server(const int port, Server::TRequestHandler handleRequest)
    : handleRequest(handleRequest)
{
    listen_socket_fd = Server::socket();
    struct sockaddr_in addr = Server::buildAddr(port);
    Server::bind(listen_socket_fd, addr);
    Server::listen(listen_socket_fd);
    std::cout << "server is running on: " << inet_ntoa(addr.sin_addr) << ':'
              << ntohs(addr.sin_port) << '\n';
}

int Server::buildFdSet(fd_set& readfds)
{
    FD_ZERO(&readfds);
    FD_SET(listen_socket_fd, &readfds);
    int max_fd = listen_socket_fd;
    for (const auto& fd : client_fds) {
        FD_SET(fd, &readfds);
        if (fd > max_fd)
            max_fd = fd;
    }
    return max_fd;
}

void Server::processNewConnection()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    const int client_fd = accept(listen_socket_fd, (struct sockaddr*)&addr, &len);
    if (client_fd == -1) {
        perror("accept call error");
        throw std::runtime_error("accept call error");
    }

    client_fds.insert(client_fd);
    std::cout << "A mate has joined (" << inet_ntoa(addr.sin_addr) << ":" << addr.sin_port << ") --- " << client_fds.size() << " client currently connected" << std::endl;
}

void Server::processRawRequest(const int fd)
{
    char buf[4];
    std::size_t bytes = recv(fd, buf, sizeof(buf), 0);

    if (bytes < sizeof(buf)) {
        throw std::runtime_error("client closed connection");
    }

    std::uint32_t msg_size = char_to_size(buf);

    std::vector<char> msg(msg_size);
    std::uint32_t bytes_read = 0;

    while (bytes_read < msg_size) {
        bytes = recv(fd, msg.data() + bytes_read, msg_size - bytes_read, 0);
        bytes_read += bytes;
        if (!bytes) {
            throw std::runtime_error("wrong message");
        }
    }

    auto res = handleRequest(msg, fd, client_fds);
    if (!res) {
        throw std::runtime_error("handler initiative");
    }

    return;
}

void Server::start_processing()
{
    fd_set readfds;
    for (;;) {
        int max_fd = buildFdSet(readfds);

        if (pselect(max_fd + 1, &readfds, NULL, NULL, NULL, NULL) == -1)
            throw std::runtime_error("pselect call error");

        if (FD_ISSET(listen_socket_fd, &readfds))
            processNewConnection();

        scheduleClientRequests(readfds);
    }
}

void Server::scheduleClientRequests(fd_set& readfds)
{
    auto it = client_fds.begin();
    while (it != client_fds.end()) {
        auto fd = *it;
        if (fd && FD_ISSET(fd, &readfds)) {
            try {
                processRawRequest(fd);
                it++;
            } catch (const std::exception& e) {
                close(fd);
                it = client_fds.erase(it);
                std::cout << "A mate has left (fd = " << fd << ", " << e.what() << ") --- " << client_fds.size() << " client currently connected" << std::endl;
            }
        } else
            it++;
    }
}

int Server::socket()
{
    int fd;
    if ((fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("failed socket call");
    return fd;
}

struct sockaddr_in Server::buildAddr(const int port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    std::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    return addr;
}

void Server::bind(const int listen_socket_fd, const struct sockaddr_in& addr)
{
    if (::bind(listen_socket_fd, (struct sockaddr*)&addr, sizeof addr) == -1)
        throw std::runtime_error("bind call error");
}

Server::~Server()
{
    if (listen_socket_fd != -1)
        close(listen_socket_fd);
    for (auto& fd : client_fds)
        close(fd);
    std::cout << "Server is down\n";
}

void Server::listen(const int listen_fd)
{
    if (::listen(listen_fd, SOMAXCONN) == -1)
        throw std::runtime_error("listen failed");
}
