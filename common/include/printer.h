#pragma once
#include "immutable_state.h"
#include "mutable_state.h"
#include "executer.h"
#include "char_processor.h"

class CursesPrinter {
public:
    void printAll(
        ImmutableState & istate,
        const MutableState & mstate,
        CursesChProcessor::ActionType action_type = CursesChProcessor::ActionType::Input) const;
};
