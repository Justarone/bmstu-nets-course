#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <curses.h>
#include <future>
#include <mutex>
#include <signal.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "executer.h"
#include "helpers.h"
#include "immutable_state.h"
#include "message.h"
#include "message_processor.h"
#include "mutable_state.h"
#include "printer.h"

int main()
{
    Client client("127.0.0.1", 3000);

    std::shared_mutex states_mutex, printer_mutex;
    auto [imstate, mstate] = client.requestStates();

    CursesPrinter printer;
    CursesChProcessor processor;
    MessageProcessor msg_processor;

    initscr();
    noecho();
    keypad(stdscr, true);

    printer.printAll(imstate, mstate);

    const auto input_proc_task = [&]() {
        while (true) {
            int ch = getch();
            CursesChProcessor::ActionType action_type;
            std::size_t pos = 0;
            {
                std::unique_lock l(states_mutex);
                action_type = processor.process(imstate, mstate, ch);
                // можно делать под shared_lock
                pos = mstate.getCurpos();
            }

            std::unique_lock lp(printer_mutex);
            std::shared_lock l(states_mutex);
            if (action_type == CursesChProcessor::ActionType::Input) {
                client.sendMessage(ch, pos);
                printer.printAll(imstate, mstate, true);
            } else if (action_type == CursesChProcessor::ActionType::Scroll) {
                printer.printAll(imstate, mstate, false);
            }
        }
    };

    auto fut = std::async(std::launch::async, input_proc_task);

    while (true) {
        Message msg;
        try {
            msg = client.recvMessage();
            std::unique_lock l(states_mutex);
            msg_processor(imstate, mstate, msg);
        } catch (boost::archive::archive_exception & e) {
            // в recvRawMessage отрабатывает recv с говном на входе при resize в i3
        }
        {
            std::unique_lock lp(printer_mutex);
            std::shared_lock l(states_mutex);
            printer.printAll(imstate, mstate, false);
        }
    }

    endwin();
    return 0;
}
