#include <bits/stdc++.h>
#include <curses.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>

#include "mutable_state.h"
#include "immutable_state.h"
#include "executer.h"
#include "char_processor.h"
#include "printer.h"

int main() {
    auto handler = (struct sigaction){SIG_IGN};
    sigaction(SIGPIPE, &handler, NULL);

    ImmutableState imstate;
    MutableState mstate;

    CursesPrinter printer;
    CursesChProcessor processor;
    BoostExecuter executer;

    initscr();
    noecho();
    keypad(stdscr, true);

    printer.printAll(imstate, mstate);

    while (true) {
        int ch = getch();
        auto action_type = processor.process(imstate, mstate, ch);
        if (action_type == CursesChProcessor::ActionType::Exit)
            break;
        else if (action_type == CursesChProcessor::ActionType::Exec) {
            auto command = mstate.getData();
            auto res = executer.execute(command);
            imstate.insertLine("> " + command);
            for (auto & s : res)
                imstate.insertLine(s);
        }
        printer.printAll(imstate, mstate, action_type);
    }

    endwin();
    return 0;
}
