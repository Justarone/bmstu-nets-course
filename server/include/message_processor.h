#include <boost/archive/basic_archive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <cstdint>
#include <curses.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_set>

#include "char_processor.h"
#include "executer.h"
#include "helpers.h"
#include "immutable_state.h"
#include "message.h"
#include "mutable_state.h"
#include "printer.h"

template <typename Executer>
class MessageProcessor {
    ImmutableState imstate;
    MutableState mstate;
    CursesChProcessor processor;
    Executer executer;

public:
    MessageProcessor() = default;
    bool operator()(const std::vector<char>& data, const int fd, const std::unordered_set<int>& all_fds);

private:
    bool processAddChar(const Message& msg, const int fd, std::unordered_set<int> all_fds);
    bool processGetAll(const int fd);
};

#include "message_processor.hpp"
