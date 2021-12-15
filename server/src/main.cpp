#include <bits/stdc++.h>
#include <boost/process/search_path.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <curses.h>
#include <sstream>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>
#include <signal.h>

#include "mutable_state.h"
#include "immutable_state.h"

namespace bp = boost::process;
namespace bio = boost::iostreams;

void printAll(ImmutableState & istate, const MutableState & mstate, std::size_t lines) {
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

std::string get_command(const std::string &full_command) {
    std::istringstream is(full_command);
    std::string command;
    is >> command;
    return command;
}

std::vector<std::string> boost_execute(const std::string & command)
{
    if (command.find_first_not_of(" \t\n") == std::string::npos)
        return {};
    auto comm = get_command(command);
    auto checked_comm = bp::search_path(comm);
    if (checked_comm.empty())
        return {"Command not found"};
     std::future<std::vector<char> > output, error;

    boost::asio::io_context svc;
    bp::child c(command, bp::std_out > output, bp::std_err > error, svc);
    svc.run();

    auto raw = output.get();
    std::vector<std::string> data;
    std::string line;
    bio::stream_buffer<bio::array_source> sb(raw.data(), raw.size());
    std::istream is(&sb);

    while (!is.eof() && std::getline(is, line))
        data.push_back(line);


    return data;
}

int main() {
    auto handler = (struct sigaction){SIG_IGN};
    sigaction(SIGPIPE, &handler, NULL);
    ImmutableState imstate;
    MutableState mstate;

    initscr();
    noecho();
    keypad(stdscr, true);
    printAll(imstate, mstate, 1);

    while(true)
    {
        int ch = getch();
        bool scroll_to_end = false;
        switch(ch)
        {
            case KEY_UP:
                imstate.moveUp();
                break;
            case KEY_DOWN:
                imstate.moveDown();
                break;
            case KEY_LEFT:
                mstate.moveLeft();
                scroll_to_end = true;
                break;
            case KEY_RIGHT:
                mstate.moveRight();
                scroll_to_end = true;
                break;
            case KEY_DC:
            case 127:
            case KEY_BACKSPACE:
                mstate.removeChar();
                scroll_to_end = true;
                break;
            case '\n': {
               auto command = mstate.getData();
               auto res = boost_execute(command);
               imstate.insertLine("> " + command);
               for (auto & s : res)
                   imstate.insertLine(s);
               scroll_to_end = true;
               break;
           }
            default:
               if(isprint(ch))
                   mstate.addChar(ch);
               scroll_to_end = true;
        }
        std::size_t y = std::max(1, getmaxy(stdscr));
        if (scroll_to_end && imstate.linesToShow() > y - 1)
           imstate.setLinesToShow(y - 1);
        printAll(imstate, mstate, y);
    }
    endwin();
    return 0;
}
