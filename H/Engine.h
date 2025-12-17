#pragma once
#include "ICommand.h"
#include <map>
#include <string>
#include <stdexcept>
#include <memory>

class Engine {
private:
    std::map<std::string, std::unique_ptr<ICommand>> commands;

public:
    void register_command(std::unique_ptr<ICommand> command, const std::string& commandName) {
        if (command) {
            commands[commandName] = std::move(command);
        }
    }

    int execute(const std::string& commandName, const std::map<std::string, int>& args) {
        auto it = commands.find(commandName);
        if (it != commands.end()) {
            return it->second->execute(args);
        }
        throw std::runtime_error("Command not found: " + commandName);
    }
};