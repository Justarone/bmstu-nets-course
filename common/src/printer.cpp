#include "printer.h"

void CursesPrinter::printAll(
        ImmutableState & istate, const MutableState & mstate, CursesChProcessor::ActionType action_type) const {
    std::size_t lines = std::max(1, getmaxy(stdscr));
    if (action_type == CursesChProcessor::ActionType::Input && istate.linesToShow() > lines - 1)
        istate.setLinesToShow(lines - 1);
    auto printed = istate.printLines(lines);

    for (std::size_t i = printed + 1; i < lines; i++) {
        move(i, 0);
        clrtoeol();
    }

    if (printed < lines) {
        mstate.printLine(printed);
        move(printed, PROMPT_SIZE + mstate.getCurpos());
    } else {
        move(0, 0);
    }
}


