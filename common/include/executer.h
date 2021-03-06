#pragma once
#include <future>
#include <sstream>
#include <string>
#include <vector>

class CommandsExecuter {
public:
    virtual std::vector<std::string> execute(const std::string& command) const = 0;
};

class BoostExecuter : public CommandsExecuter {
    std::string get_command(const std::string& full_command) const;

public:
    std::vector<std::string> execute(const std::string& command) const override;
};
