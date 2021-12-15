#include "message.h"

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

    // write default first
    outs << std::uint32_t();
    boost::archive::text_oarchive out(outs);
    out << *this;
    auto res = outs.str();

    // convert size to string
    auto size = res.size() - sizeof(std::uint32_t);
    std::ostringstream sizeout;
    sizeout << size;
    //
    // rewrite default
    res.replace(0, sizeof(std::uint32_t), sizeout.str());

    return res;
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

Message AddCharMessage::buildMessage() {
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

Message DiffMessage::buildMessage() {
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

Message GetAllMessage::buildMessage() {
    std::ostringstream outs;
    boost::archive::text_oarchive out(outs);
    out << *this;
    return Message { Message::Type::diff, outs.str() };
}

GetAllMessage::GetAllMessage(const ImmutableState & imstate, const MutableState & mstate) : imstate(imstate), mstate(mstate) {}
