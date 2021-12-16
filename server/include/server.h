#pragma once
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Server {
public:
    using TRequestHandler = std::function<bool(const std::vector<char>&, const int, const std::unordered_set<int>&)>;

private:
    int listen_socket_fd = -1;
    std::unordered_set<int> client_fds;
    TRequestHandler handleRequest;

public:
    Server(const int port, TRequestHandler handleRequest);
    void start_processing();
    ~Server();

protected:
    static struct sockaddr_in buildAddr(const int port);
    static int socket();
    static void bind(const int listen_socket_fd, const struct sockaddr_in& addr);
    static void listen(const int listen_fd);
    int buildFdSet(fd_set& readfds);
    void processNewConnection();
    void processRawRequest(const int fd);
    void scheduleClientRequests(fd_set& readfds);
};
