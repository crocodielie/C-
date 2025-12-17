#include "Tests.h"
#include "Engine.h"
#include "Wrapper.h"
#include "Subject.h"
#include <iostream>
#include <cassert>
#include <functional>
#include <memory>
#include <map>

// Вспомогательная функция для тестирования
void run_test(const std::string& testName, std::function<void()> testBody) {
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << "TEST START: " << testName << "\n";
    std::cout << "------------------------------------------------------------\n";
    try {
        testBody();
        std::cout << ">>> TEST RESULT: [SUCCESS]\n";
    }
    catch (const std::exception& e) {
        std::cout << ">>> TEST RESULT: [FAILURE] Exception: " << e.what() << "\n";
    }
}

// Тест 1: стандартный метод с 2 аргументами
void test_standard_2_args() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"arg1", 0}, { "arg2", 0 }})),
        "command1");

    int result = engine.execute("command1", std::map<std::string, int>{{"arg1", 4}, { "arg2", 5 }});
    std::cout << "   Expected: 9\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != 9) throw std::logic_error("Calculation error");
}

// Тест 2: метод с 4 аргументами
void test_4_args_method() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int, int, int>(
            &subj, &Subject::sum_4_args,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }, { "c", 0 }, { "d", 0 }})),
        "sum4");

    int result = engine.execute("sum4", std::map<std::string, int>{
        {"a", 1}, { "b", 2 }, { "c", 3 }, { "d", 4 }});
    std::cout << "   Expected: 10\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != 10) throw std::logic_error("Calculation error");
}

// Тест 3: метод без аргументов
void test_zero_args_method() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int>(
            &subj, &Subject::f_zero, std::map<std::string, int>{})),
        "zero");

    int result = engine.execute("zero", std::map<std::string, int>{});
    std::cout << "   Expected: 42\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != 42) throw std::logic_error("Value mismatch");
}

// Тест 4: обработка ошибок - неизвестный аргумент
void test_missing_argument_error() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int, int, int>(
            &subj, &Subject::sum_4_args,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }, { "c", 0 }, { "d", 0 }})),
        "sum4");

    engine.execute("sum4", std::map<std::string, int>{{"a", 10}, { "c", 20 }});
    throw std::logic_error("Should have thrown an exception");
}

// Тест 5: обработка ошибок - неизвестная команда
void test_unknown_command_error() {
    Subject subj;
    Engine engine;

    engine.execute("non_existent", std::map<std::string, int>{{"x", 10}});
    throw std::logic_error("Should have thrown an exception");
}

// Тест 6: повторная регистрация команды
void test_re_register_command() {
    Subject subj;
    Engine engine;

    // Первая регистрация
    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"arg1", 0}, { "arg2", 0 }})),
        "calc");

    // Вторая регистрация с тем же именем (должна заменить предыдущую)
    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"x", 0}, { "y", 0 }})),
        "calc");

    // Должно работать с новой сигнатурой аргументов
    int result = engine.execute("calc", std::map<std::string, int>{{"x", 3}, { "y", 7 }});
    std::cout << "   Expected: 10\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != 10) throw std::logic_error("Re-registration failed");
}

// Тест 7: метод с отрицательными числами
void test_negative_numbers() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }})),
        "subtract");

    int result = engine.execute("subtract", std::map<std::string, int>{{"a", -5}, { "b", 3 }});
    std::cout << "   Expected: -2\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != -2) throw std::logic_error("Negative numbers calculation error");
}

// Тест 8: несколько команд в одном Engine
void test_multiple_commands() {
    Subject subj;
    Engine engine;

    // Регистрируем несколько команд
    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }})),
        "add");

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int, int, int>(
            &subj, &Subject::sum_4_args,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }, { "c", 0 }, { "d", 0 }})),
        "sum4");

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int>(
            &subj, &Subject::f_zero, std::map<std::string, int>{})),
        "magic");

    // Выполняем все команды
    int result1 = engine.execute("add", std::map<std::string, int>{{"a", 10}, { "b", 20 }});
    int result2 = engine.execute("sum4", std::map<std::string, int>{
        {"a", 1}, { "b", 2 }, { "c", 3 }, { "d", 4 }});
    int result3 = engine.execute("magic", std::map<std::string, int>{});

    std::cout << "   Result add: " << result1 << " (expected: 30)\n";
    std::cout << "   Result sum4: " << result2 << " (expected: 10)\n";
    std::cout << "   Result magic: " << result3 << " (expected: 42)\n";

    if (result1 != 30 || result2 != 10 || result3 != 42) {
        throw std::logic_error("Multiple commands test failed");
    }
}

// Тест 9: неправильный тип аргументов (должно быть исключение при компиляции или выполнении)
void test_wrong_argument_count() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int, int, int>(
            &subj, &Subject::f3,
            std::map<std::string, int>{{"a", 0}, { "b", 0 }})),
        "add");

    // Пытаемся передать только один аргумент вместо двух
    engine.execute("add", std::map<std::string, int>{{"a", 10}});
    throw std::logic_error("Should have thrown an exception for wrong argument count");
}

// Тест 10: пустое имя команды
void test_empty_command_name() {
    Subject subj;
    Engine engine;

    engine.register_command(
        std::unique_ptr<ICommand>(new Wrapper<Subject, int>(
            &subj, &Subject::f_zero, std::map<std::string, int>{})),
        "");

    // Пытаемся выполнить команду с пустым именем
    int result = engine.execute("", std::map<std::string, int>{});
    std::cout << "   Expected: 42\n";
    std::cout << "   Actual:   " << result << "\n";
    if (result != 42) throw std::logic_error("Empty command name test failed");
}

// Функция для запуска всех тестов
void run_all_tests() {
    std::cout << "============================================================" << std::endl;
    std::cout << "           STARTING ALL TESTS                               " << std::endl;
    std::cout << "============================================================" << std::endl;

    // Оригинальные 5 тестов
    run_test("1. Standard 2-Argument Method", test_standard_2_args);
    run_test("2. 4-Argument Method", test_4_args_method);
    run_test("3. Zero Arguments Method", test_zero_args_method);
    run_test("4. Error Handling: Missing Argument Key", []() {
        try {
            test_missing_argument_error();
        }
        catch (const std::exception& e) {
            std::cout << "   Correctly caught exception: " << e.what() << "\n";
        }
        });
    run_test("5. Error Handling: Unknown Command", []() {
        try {
            test_unknown_command_error();
        }
        catch (const std::exception& e) {
            std::cout << "   Correctly caught exception: " << e.what() << "\n";
        }
        });

    // Новые 5 тестов
    run_test("6. Re-register Command", test_re_register_command);
    run_test("7. Negative Numbers Calculation", test_negative_numbers);
    run_test("8. Multiple Commands in One Engine", test_multiple_commands);
    run_test("9. Error Handling: Wrong Argument Count", []() {
        try {
            test_wrong_argument_count();
        }
        catch (const std::exception& e) {
            std::cout << "   Correctly caught exception: " << e.what() << "\n";
        }
        });
    run_test("10. Empty Command Name", test_empty_command_name);

    std::cout << "\n============================================================" << std::endl;
    std::cout << "           ALL TESTS COMPLETED                              " << std::endl;
    std::cout << "============================================================" << std::endl;
}