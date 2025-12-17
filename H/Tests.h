#pragma once
#include <string>
#include <functional>

void run_test(const std::string& testName, std::function<void()> testBody);
void run_all_tests();