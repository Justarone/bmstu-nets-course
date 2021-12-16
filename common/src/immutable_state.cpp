#include "immutable_state.h"
#include <curses.h>

std::size_t ImmutableState::printLines(
        const std::size_t max_lines,
        const std::size_t offset) const
{
    auto limit = std::min(max_lines, data.size() - startline);
    for(std::size_t i = 0; i < limit; i++) {
        move(i + offset, 0);
        clrtoeol();
        printw("%s", data[i + startline].c_str());
    }
    return limit;
}

void ImmutableState::insertLine(const std::string & s) {
    data.push_back(s);
}

void ImmutableState::insertLines(std::vector<std::string> && vs) {
    for (std::size_t i = 0; i < vs.size(); i++)
        data.push_back(std::move(vs[i]));
}

void ImmutableState::moveUp(const std::size_t diff) {
    startline = diff > startline ? 0 : startline - diff;
}

void ImmutableState::moveDown(const std::size_t diff) {
    startline = std::min(data.size(), startline + diff);
}

std::size_t ImmutableState::linesToShow() {
    return data.size() - startline;
}

void ImmutableState::setLinesToShow(const std::size_t lines) {
    startline = data.size() > lines ? data.size() - lines : 0;
}
