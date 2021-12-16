#include "printer.h"
#include <fstream>

void CursesPrinter::printAll(ImmutableState & istate, const MutableState & mstate, bool moveToActual) const {
    std::size_t lines = std::max(1, getmaxy(stdscr));

    if (moveToActual && istate.linesToShow() > lines - 1)
        istate.setLinesToShow(lines - 1);
    auto printed = istate.printLines(lines);

    // убрать, если нужен дебаг
    for (std::size_t i = printed + 1; i < lines; i++) {
        move(i, 0);
        clrtoeol();
    }

    if (printed < lines) {
        mstate.printLine(printed);
    } else {
        move(0, 0);
    }

    refresh();
}


