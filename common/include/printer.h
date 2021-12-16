#pragma once
#include "char_processor.h"
#include "executer.h"
#include "immutable_state.h"
#include "mutable_state.h"

class CursesPrinter {
public:
    void printAll(ImmutableState& istate, const MutableState& mstate, bool moveToActual = true) const;
};
