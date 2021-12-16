#include "helpers.h"
#include <curses.h>

void broadcastPacket(const std::string& packet, const std::unordered_set<int>& all_fds)
{
    for (const auto& fd : all_fds) {
        if (send(fd, packet.data(), packet.size(), 0) == -1)
            std::cout << "Can't broadcast packet to client (fd = " << fd << ")\n";
    }
}

std::uint32_t char_to_size(const char buf[4])
{
    std::uint32_t size = 0;
    for (int i = 0; i < 4; i++) {
        size |= static_cast<unsigned char>(buf[4 - i - 1]);
        if (i != 3)
            size <<= 8;
    }
    return size;
}

void size_to_char(std::uint32_t size, char* buf)
{
    for (int i = 0; i < 4; i++, size >>= 8)
        buf[i] = size & 0xff;
}

bool is_backspace(const int ch)
{
    return ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127;
}
