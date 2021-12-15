#include "helpers.h"

void broadcastPacket(const std::string & packet, const std::unordered_set<int> & all_fds) {
    for (const auto & fd : all_fds) {
        if (send(fd, packet.data(), packet.size(), 0) == -1)
            std::cout << "Can't broadcast packet to client (fd = " << fd << ")\n";
    }
}
