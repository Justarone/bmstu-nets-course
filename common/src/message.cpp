#include "message.h"
#include "helpers.h"

template <typename Archive>
void Message::serialize(Archive & ar, const unsigned int version) {
    ar & type;
    ar & data;
}

Message::Message(const std::string & s) {
    std::istringstream ins(s);
    boost::archive::text_iarchive in(ins);
    in >> *this;
}

Message::Message(Message::Type type, const std::string && s) : type(type), data(std::move(s)) {}

std::string Message::buildPacket() const {
    std::ostringstream outs;

    boost::archive::text_oarchive out(outs);
    out << *this;
    auto res = outs.str();

    char buf[4];
    size_to_char(res.size(), buf);
    std::string prefix = { buf, 4 };
    return prefix + res;
}

template <typename Archive>
void AddCharMessage::serialize(Archive & ar, const unsigned int version) {
    ar & pos;
    ar & data;
}

AddCharMessage::AddCharMessage(const std::string & s) {
    std::istringstream ins(s);
    boost::archive::text_iarchive in(ins);
    in >> *this;
}

AddCharMessage::AddCharMessage(const int data, const std::size_t pos) : pos(pos), data(data) {}

Message AddCharMessage::buildMessage() const {
    std::ostringstream outs;
    boost::archive::text_oarchive out(outs);
    out << *this;
    return Message { Message::Type::addChar, outs.str() };
}

template <typename Archive>
void DiffMessage::serialize(Archive & ar, const unsigned int version) {
    ar & diff;
}

DiffMessage::DiffMessage(const std::vector<std::string> & diff) : diff(diff) {}

DiffMessage::DiffMessage(const std::string & s) {
    std::istringstream ins(s);
    boost::archive::text_iarchive in(ins);
    in >> *this;
}

Message DiffMessage::buildMessage() const {
    std::ostringstream outs;
    boost::archive::text_oarchive out(outs);
    out << *this;
    return Message { Message::Type::diff, outs.str() };
}

template <typename Archive>
void GetAllMessage::serialize(Archive & ar, const unsigned int version) {
    ar & imstate;
    ar & mstate;
}

GetAllMessage::GetAllMessage(const std::string & s) {
    std::istringstream ins(s);
    boost::archive::text_iarchive in(ins);
    in >> *this;
}

Message GetAllMessage::buildMessage() const {
    std::ostringstream outs;
    boost::archive::text_oarchive out(outs);
    out << *this;
    return Message { Message::Type::getAll, outs.str() };
}

GetAllMessage::GetAllMessage(const ImmutableState & imstate, const MutableState & mstate) : imstate(imstate), mstate(mstate) {}
