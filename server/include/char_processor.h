#pragma once
#include "executer.h"
#include "immutable_state.h"
#include "mutable_state.h"
#include <curses.h>

class CursesChProcessor {
public:
    enum class ActionType {
        BadKey,
        Input,
        Exit,
        Exec
    };
    ActionType process(ImmutableState& istate, MutableState& mstate, const int ch) const;
};
