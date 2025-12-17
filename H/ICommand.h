#pragma once
#include <map>
#include <string>

// Интерфейс команды
class ICommand {
public:
    virtual int execute(const std::map<std::string, int>& args) = 0;
    virtual ~ICommand() = default;
};