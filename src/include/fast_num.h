#pragma once

#include <vector>
#include <array>
#include <optional>
#include <string>


#include "utility.h"


namespace fast_ops {
    int add(int a, int b);
    int sub(int a, int b);
    int mul(int a, int b);
    int div(int a, int b);
}
using operation = int(int, int);

std::vector<Solution> search(std::array<int, 6> numbers, int goal);

struct Operation { char as_char; int priority; std::optional<int> (*function)(int,int); };

//Correct operations, but slower
constexpr auto operations = std::to_array<Operation>({
        {'+', 1, [] (int a, int b) -> std::optional<int> { return a + b; } },
        {'-', 1, [] (int a, int b) -> std::optional<int> { return a - b; } },
        {'*', 2, [] (int a, int b) -> std::optional<int> { return a * b; } },
        {'/', 2, [] (int a, int b) -> std::optional<int> { if (!b || (a % b)) return std::nullopt; return a / b;} }
    });

struct Stack_element {
    int value;
    int priority;
    std::string as_string;
};

std::vector<Solution> validate(std::vector<Solution> &solutions);

std::vector<Solution> optimize(std::vector<Solution> &solutions);