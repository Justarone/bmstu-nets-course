#include <bits/stdc++.h>
#include <curses.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <future>
#include <mutex>

#include "mutable_state.h"
#include "immutable_state.h"
#include "executer.h"
#include "char_processor.h"
#include "printer.h"
#include "message.h"
#include "helpers.h"
#include "client.h"
#include "message_processor.h"

int main() {
    Client client("127.0.0.1", 3000);

    std::shared_mutex states_mutex, printer_mutex;
    auto [imstate, mstate] = client.requestStates();

    CursesPrinter printer;
    //CursesChProcessor processor;
    MessageProcessor msg_processor;

    initscr();
    noecho();
    keypad(stdscr, true);

    const auto input_proc_task = [&]() {
        while (true) {
            int ch = getch();
            client.sendMessage(ch, mstate.getCurpos());
        }
    };

    auto fut = std::async(std::launch::async, input_proc_task);

    printer.printAll(imstate, mstate);

    while (true) {
        auto msg = client.recvMessage();
        {
            std::unique_lock l(states_mutex);
            msg_processor(imstate, mstate, msg);
        }
        {
            std::shared_lock l(states_mutex);
            std::unique_lock lp(printer_mutex);
            printer.printAll(imstate, mstate, CursesChProcessor::ActionType::Input);
        }
    }

    endwin();
    return 0;
}
