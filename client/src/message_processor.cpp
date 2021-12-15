#include "message_processor.h"

void MessageProcessor::operator()(ImmutableState & imstate, MutableState & mstate, Message msg) {
    if (msg.type == Message::Type::addChar) {
        const auto message = AddCharMessage(msg.data);
        const auto old_pos = mstate.getCurpos();
        mstate.setCurpos(message.pos);
        mstate.addChar(static_cast<char>(message.data));
        mstate.setCurpos(old_pos);
    } else if (msg.type == Message::Type::diff) {
        auto message = DiffMessage(msg.data);
        mstate.getData();
        imstate.insertLines(std::move(message.diff));
    }
}
