#include "char_processor.h"

CursesChProcessor::ActionType CursesChProcessor::process(ImmutableState & imstate, MutableState & mstate, CommandsExecuter & executer) const {
    int ch = getch();
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
            auto command = mstate.getData();
            auto res = executer.execute(command);
            imstate.insertLine("> " + command);
            for (auto & s : res)
                imstate.insertLine(s);
            input_event = true;
            break;
        }
        default:
            if (isprint(ch))
                mstate.addChar(ch);
            input_event = true;
    }
    return input_event ? ActionType::Input : ActionType::Scroll;
}
