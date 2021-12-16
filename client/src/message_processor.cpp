#include "message_processor.h"
#include <curses.h>

void MessageProcessor::operator()(ImmutableState& imstate, MutableState& mstate, Message msg)
{
    if (msg.type == Message::Type::addChar) {
        const auto message = AddCharMessage(msg.data);
        const auto old_pos = mstate.getCurpos();

        mstate.setCurpos(message.pos);
        if (is_backspace(message.data))
            mstate.removeChar();
        else
            mstate.addChar(static_cast<char>(message.data));
        movePos(mstate, old_pos, message.pos, message.data);

        // пример дебага
        // move(50, 0);
        // printw("Debug message....");

    } else if (msg.type == Message::Type::diff) {
        auto message = DiffMessage(msg.data);
        mstate.getData();
        imstate.insertLines(std::move(message.diff));
    }
}

void MessageProcessor::movePos(MutableState& mstate, const std::size_t old_pos, const std::size_t pos, const int data)
{
    std::size_t new_pos;
    if (data == KEY_BACKSPACE || data == 127 || data == KEY_DC)
        new_pos = (old_pos >= pos && old_pos > 0) ? old_pos - 1 : old_pos;
    else
        new_pos = (old_pos < pos) ? old_pos : old_pos + 1;
    mstate.setCurpos(new_pos);
}
