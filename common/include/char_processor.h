#pragma once
#include <curses.h>
#include "immutable_state.h"
#include "mutable_state.h"
#include "executer.h"

class CursesChProcessor {
public:
    enum class ActionType {
        Scroll,
        Input,
        Exit
    };
    ActionType process(ImmutableState & istate, MutableState & mstate, CommandsExecuter & executer) const;
};
