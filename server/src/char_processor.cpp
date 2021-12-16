#include "char_processor.h"
#include <iostream>

CursesChProcessor::ActionType CursesChProcessor::process(ImmutableState& imstate, MutableState& mstate, const int ch) const
{
    bool input_event = false;

    switch (ch) {
    case KEY_DC:
    case 127:
    case KEY_BACKSPACE:
        mstate.removeChar();
        input_event = true;
        break;
    case '\n': {
        std::cout << "exec: "
                  << mstate.getText() << std::endl;
        return ActionType::Exec;
        break;
    }
    default:
        if (isprint(ch)) {
            mstate.addChar(ch);
            input_event = true;
        }
    }
    return input_event ? ActionType::Input : ActionType::BadKey;
}
