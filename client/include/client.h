#pragma once
#include <arpa/inet.h>
#include <curses.h>
#include <sys/socket.h>

#include "helpers.h"
#include "immutable_state.h"
#include "message.h"
#include "mutable_state.h"

class Client {
    int sfd;

public:
    Client(const std::string& host, const int port);
    ~Client();
    std::pair<ImmutableState, MutableState> requestStates() const;
    void sendMessage(const int ch, const std::size_t pos);
    Message recvMessage() const;
    std::string recvRawMessage() const;
};
