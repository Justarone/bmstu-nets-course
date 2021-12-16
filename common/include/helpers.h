#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unordered_set>

void broadcastPacket(const std::string& packet, const std::unordered_set<int>& all_fds);
std::uint32_t char_to_size(const char buf[4]);
void size_to_char(std::uint32_t size, char* buf);
bool is_backspace(const int ch);
