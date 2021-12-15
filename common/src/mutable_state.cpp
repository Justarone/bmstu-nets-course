#include "mutable_state.h"
#include <curses.h>

std::size_t MutableState::getCurpos() const { return curpos; }

void MutableState::setCurpos(const std::size_t new_curpos) { curpos = std::max(new_curpos, data.size()); }

void MutableState::printLine(const std::size_t yoffset) const {
    move(yoffset, 0);
    clrtoeol();
    printw(PROMPT "%s", data.c_str());
}

void MutableState::addChar(const char c) {
    data.insert(data.begin() + curpos++, c);
}

void MutableState::removeChar() {
    if (curpos > 0)
        data.erase(--curpos, 1);
}

void MutableState::moveLeft(const std::size_t diff) {
    curpos = diff > curpos ? 0 : curpos - diff;
}

void MutableState::moveRight(const std::size_t diff) {
    curpos = std::min(data.size(), curpos + diff);
}

std::string MutableState::getData() {
    curpos = 0;
    auto old_data = std::move(data);
    data.clear();
    return old_data;
}
