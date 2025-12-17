#pragma once
#include <string>
#include <iostream>

class Subject {
public:
    // Пример из презентации
    int f3(int a, int b) {
        std::cout << "[Subject::f3] " << a << " + " << b << " = " << (a + b) << std::endl;
        return a + b;
    }

    // Метод с 4 аргументами
    int sum_4_args(int a, int b, int c, int d) {
        std::cout << "[Subject::sum_4_args] " << a << " + " << b << " + " << c << " + " << d
            << " = " << (a + b + c + d) << std::endl;
        return a + b + c + d;
    }

    // Метод без аргументов
    int f_zero() {
        std::cout << "[Subject::f_zero] returning 42" << std::endl;
        return 42;
    }
};