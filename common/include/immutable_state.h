#pragma once
#include <string>
#include <numeric>
#include <vector>
#include <limits>
#include <boost/serialization/access.hpp>

class ImmutableState {
    std::size_t startline = 0;
    std::vector<std::string> data;
public:
    std::size_t printLines(
        const std::size_t max_lines = std::numeric_limits<std::size_t>::max(),
        const std::size_t offset = 0) const;
    void insertLine(const std::string & s);
    void insertLines(std::vector<std::string> && vs);
    void moveUp(const std::size_t diff = 1);
    void moveDown(const std::size_t diff = 1);
    std::size_t linesToShow();
    void setLinesToShow(const std::size_t lines);
private:
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & data;
    }
};
