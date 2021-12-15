#include "executer.h"
#include <boost/process/search_path.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

namespace bp = boost::process;
namespace bio = boost::iostreams;

std::string BoostExecuter::get_command(const std::string &full_command) const {
    std::istringstream is(full_command);
    std::string command;
    is >> command;
    return command;
}

std::vector<std::string> BoostExecuter::execute(const std::string & command) const {
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

