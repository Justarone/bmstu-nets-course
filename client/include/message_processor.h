#pragma once

#include "immutable_state.h"
#include "mutable_state.h"
#include "message.h"
#include "helpers.h"

class MessageProcessor {
public:
    void operator()(ImmutableState & imstate, MutableState & mstate, Message msg);
private:
    void movePos(MutableState & msate, const std::size_t old_pos, const std::size_t pos, const int data);
};
