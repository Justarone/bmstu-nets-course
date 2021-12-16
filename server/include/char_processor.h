#pragma once
#include <curses.h>
#include "immutable_state.h"
#include "mutable_state.h"
#include "executer.h"

class CursesChProcessor {
public:
    enum class ActionType {
        BadKey,
        Input,
        Exit,
        Exec
    };
    ActionType process(ImmutableState & istate, MutableState & mstate, const int ch) const;
};
