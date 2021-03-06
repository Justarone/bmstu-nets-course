#pragma once
#include <boost/serialization/access.hpp>
#include <numeric>
#include <string>

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
    const std::string& getText() const;
    void clearData();
    std::string getData();

private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& data;
    }
};
