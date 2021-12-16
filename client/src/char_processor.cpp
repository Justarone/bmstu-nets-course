#include "char_processor.h"
#include <iostream>

CursesChProcessor::ActionType CursesChProcessor::process(ImmutableState & imstate, MutableState & mstate, const int ch) const {
    bool input_event = false;

    switch (ch)
    {
        case KEY_UP:
            imstate.moveUp();
            return ActionType::Scroll;
        case KEY_DOWN:
            imstate.moveDown();
            return ActionType::Scroll;
        case KEY_LEFT:
            mstate.moveLeft();
            return ActionType::Scroll;
        case KEY_RIGHT:
            mstate.moveRight();
            return ActionType::Scroll;
        case KEY_DC:
        case 127:
        case KEY_BACKSPACE:
        case '\n':
            input_event = true;
            break;
        default:
            if (isprint(ch) && additionalFilter(ch))
                input_event = true;
    }
    return input_event ? ActionType::Input : ActionType::BadKey;
}

bool CursesChProcessor::additionalFilter(const int ch) {
    return ch >= std::numeric_limits<char>::min() && ch <= std::numeric_limits<char>::max();
}
