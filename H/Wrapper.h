#pragma once
#include "ICommand.h"
#include <functional>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>

// Вспомогательная структура для индексов (C++14 compatible)
template<std::size_t...>
struct index_sequence {};

template<std::size_t N, std::size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

template<std::size_t... Is>
struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

// Основной класс Wrapper
template<typename Object, typename Ret, typename... Args>
class Wrapper : public ICommand {
private:
    Object* obj;
    Ret(Object::* method)(Args...);
    std::vector<std::string> argNames;

    template<std::size_t... Is>
    Ret call_func(const std::map<std::string, int>& args, index_sequence<Is...>) {
        // Проверяем наличие всех аргументов
        for (std::size_t i = 0; i < argNames.size(); ++i) {
            if (args.find(argNames[i]) == args.end()) {
                throw std::out_of_range("Missing argument: " + argNames[i]);
            }
        }

        // Вызываем метод с распакованными аргументами
        return (obj->*method)(args.find(argNames[Is])->second...);
    }

public:
    Wrapper(Object* obj, Ret(Object::* method)(Args...),
        const std::map<std::string, int>& defaults)
        : obj(obj), method(method) {
        // Заполняем имена аргументов из defaults
        for (const auto& pair : defaults) {
            argNames.push_back(pair.first);
        }

        // Проверяем количество аргументов
        if (argNames.size() != sizeof...(Args)) {
            throw std::runtime_error("Argument count mismatch in Wrapper constructor");
        }
    }

    int execute(const std::map<std::string, int>& args) override {
        // Проверяем количество аргументов
        if (args.size() != sizeof...(Args)) {
            throw std::runtime_error("Arguments count mismatch. Expected: " +
                std::to_string(sizeof...(Args)));
        }

        // Вызываем метод
        return call_func(args, make_index_sequence<sizeof...(Args)>{});
    }
};