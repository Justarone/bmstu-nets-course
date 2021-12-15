#pragma once

#include "immutable_state.h"
#include "mutable_state.h"
#include "message.h"

class MessageProcessor {
public:
    void operator()(ImmutableState & imstate, MutableState & mstate, Message msg);
};
