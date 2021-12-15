#pragma once
#include <iostream>
#include <unordered_set>
#include <string>
#include <sys/socket.h>

void broadcastPacket(const std::string & packet, const std::unordered_set<int> & all_fds);
