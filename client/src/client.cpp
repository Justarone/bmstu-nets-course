#include "client.h"

Client::Client(const std::string& host, const int port)
{
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
        throw std::runtime_error("socket call error");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    addr.sin_port = htons(port);

    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("connect call error");
}

Client::~Client()
{
    close(sfd);
}

std::pair<ImmutableState, MutableState> Client::requestStates() const
{
    auto request = GetAllMessage({}, {});
    const auto request_message = request.buildMessage();
    auto packet = request_message.buildPacket();

    if (send(sfd, packet.data(), packet.size(), 0) == -1)
        throw std::runtime_error("Can't requests states");

    Message message;

    while (true) {
        message = recvMessage();
        if (request_message.type == Message::Type::getAll)
            break;
    }

    auto final_message = GetAllMessage(message.data);

    return { final_message.imstate, final_message.mstate };
}

void Client::sendMessage(const int ch, const std::size_t pos)
{
    const auto add_char_message = AddCharMessage(ch, pos);
    const auto message = add_char_message.buildMessage();
    const auto packet = message.buildPacket();
    if (send(sfd, packet.data(), packet.size(), 0) == -1)
        throw std::runtime_error("Can't send packet");
}

Message Client::recvMessage() const
{
    const auto data = recvRawMessage();
    return { data };
}

std::string Client::recvRawMessage() const
{
    char buf[4];
    std::size_t bytes = recv(sfd, &buf, sizeof(buf), 0);

    if (bytes < sizeof(buf))
        throw std::runtime_error("Can't get packet size");

    std::uint32_t msg_size = char_to_size(buf);
    std::vector<char> msg(msg_size);
    std::uint32_t bytes_read = 0;

    while (bytes_read < msg_size) {
        bytes = recv(sfd, msg.data() + bytes_read, msg_size - bytes_read, 0);
        if (!bytes)
            throw std::runtime_error("Wrong message\n");
        bytes_read += bytes;
    }

    return { msg.begin(), msg.end() };
}
