#include "char_processor.h"

CursesChProcessor::ActionType CursesChProcessor::process(ImmutableState & imstate, MutableState & mstate, const int ch) const {
    bool input_event = false;

    switch (ch)
    {
        case KEY_F(4):
            return ActionType::Exit;
        case KEY_UP:
            imstate.moveUp();
            break;
        case KEY_DOWN:
            imstate.moveDown();
            break;
        case KEY_LEFT:
            mstate.moveLeft();
            input_event = true;
            break;
        case KEY_RIGHT:
            mstate.moveRight();
            input_event = true;
            break;
        case KEY_DC:
        case 127:
        case KEY_BACKSPACE:
            mstate.removeChar();
            input_event = true;
            break;
        case '\n': {
            return ActionType::Exec;
            break;
        }
        default:
            if (isprint(ch))
                mstate.addChar(ch);
            input_event = true;
    }
    return input_event ? ActionType::Input : ActionType::Scroll;
}
