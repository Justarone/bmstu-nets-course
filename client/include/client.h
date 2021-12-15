#pragma once
#include <curses.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "mutable_state.h"
#include "immutable_state.h"
#include "message.h"
#include "helpers.h"

class Client {
    int sfd;
public:
    Client(const std::string & host, const int port);
    ~Client();
    std::pair<ImmutableState, MutableState> requestStates() const;
    void sendMessage(const int ch, const std::size_t pos);
    Message recvMessage() const;
    std::string recvRawMessage() const;
};
