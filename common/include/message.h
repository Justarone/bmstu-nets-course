#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include "immutable_state.h"
#include "mutable_state.h"

// TODO: serialized message may be built faster from concrete messages
struct Message {
    enum class Type : std::uint8_t {
        addChar,
        getAll,
        diff,
    };

    Type type;
    std::string data;

    Message(const std::string & s);
    Message(Message::Type type, const std::string && s);
    std::string buildPacket() const;
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version);
};


struct AddCharMessage {
    std::size_t pos;
    int data;

    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version);
    AddCharMessage(const std::string & s);
    Message buildMessage();
};


struct DiffMessage {
    std::vector<std::string> diff;

    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version);
    DiffMessage(const std::string & s);
    DiffMessage(const std::vector<std::string> & diff);
    Message buildMessage();
};


// TODO: serialization can be done with just borrowing
struct GetAllMessage {
     ImmutableState imstate;
     MutableState mstate;

    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version);
    GetAllMessage(const std::string & s);
    GetAllMessage(const ImmutableState & imstate, const MutableState & mstate);
    Message buildMessage();
};
