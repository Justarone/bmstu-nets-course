#pragma once
#include <string>
#include <numeric>
#include <vector>
#include <limits>

class ImmutableState {
    std::size_t startline = 0;
    std::vector<std::string> data;
public:
    std::size_t printLines(
        const std::size_t max_lines = std::numeric_limits<std::size_t>::max(),
        const std::size_t offset = 0) const;
    void insertLine(const std::string & s);
    void moveUp(const std::size_t diff = 1);
    void moveDown(const std::size_t diff = 1);
    std::size_t linesToShow();
    void setLinesToShow(const std::size_t lines);
};
