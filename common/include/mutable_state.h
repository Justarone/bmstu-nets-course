#pragma once
#include <numeric>
#include <string>
#include <boost/serialization/access.hpp>

#define PROMPT "> "
#define PROMPT_SIZE 2

class MutableState {
    std::string data;
    std::size_t curpos = 0;
public:
    std::size_t getCurpos() const;
    void setCurpos(const std::size_t new_curpos);

    void printLine(const std::size_t yoffset = 0) const;
    void addChar(const char c);
    void removeChar();
    void moveLeft(const std::size_t diff = 1);
    void moveRight(const std::size_t diff = 1);
    std::string getData();
    const std::string & getText() const {
        return data;
    }
private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & data;
    }
};
