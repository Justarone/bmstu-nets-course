#include <boost/archive/basic_archive.hpp>
#include <iostream>
#include <cstdint>
#include <curses.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <memory>
#include <unordered_set>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include "message.h"
#include "mutable_state.h"
#include "immutable_state.h"
#include "executer.h"
#include "char_processor.h"
#include "printer.h"
#include "helpers.h"

template <typename Executer>
class MessageProcessor {
    ImmutableState imstate;
    MutableState mstate;
    CursesChProcessor processor;
    Executer executer;
public:
    MessageProcessor() = default;
    bool operator()(const std::vector<char>& data, const int fd, const std::unordered_set<int> & all_fds);
private:
    bool processAddChar(const Message & msg, const int fd, std::unordered_set<int> all_fds);
    bool processGetAll(const int fd);
};

#include "message_processor.hpp"
