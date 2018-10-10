#include <iostream>
#include <stack>
#include <deque>
#include <regex>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <chrono>

#include "combinations.h"

enum element_type {
    ℤ, 𝕆 //number or operator
};

struct element {
    element_type type;
    char value;
};

std::ostream &operator<<(std::ostream &os, const element &el) {
    if (el.type == ℤ) os << static_cast<int>(el.value) << " ";
    if (el.type == 𝕆) os << el.value << " ";
    return os;
}

using expression = std::vector<element>;

expression operator "" _expr(const char *str, std::size_t len) {
    std::regex rgx(R"/((\d+)|([-+*/]))/");
    std::cregex_iterator elements_begin(str, str + len, rgx);
    std::cregex_iterator elements_end;

    expression result;
    std::transform(elements_begin, elements_end, std::back_inserter(result),
                   [str](std::cmatch match) {
                       if (match.length(1) > 0) {
                           return element{ℤ, static_cast<char>(std::stoi(match.str(1)))};
                       } else if (match.length(2) > 0) {
                           return element{𝕆, static_cast<char>(match.str()[0])};
                       } else {
                           throw std::logic_error("Bad expression: " + std::string(str));
                       }
                   }
    );
    return result;
}

int eval(expression expr) {
    std::stack<int> numbers;
    for (const auto &el: expr) {
        if (el.type == 𝕆) {
            int result = 0;
            int a, b;
            if (numbers.empty()) throw std::invalid_argument("Stack is empty");
            b = numbers.top();
            numbers.pop();
            if (numbers.empty()) throw std::invalid_argument("Stack is empty");
            a = numbers.top();
            numbers.pop();
            switch (el.value) {
                case '+':
                    result = a + b;
                    break;
                case '-':
                    result = a - b;
                    break;
                case '/':
                    result = a / b;
                    break;
                case '*':
                    result = a * b;
                    break;
                default:
                    throw std::logic_error("Operator is not supported: " + el.value);
            }
            numbers.push(result);
        } else {
            numbers.push(static_cast<int>(el.value));
        }
    }
    return numbers.empty() ? 0 : numbers.top();
}

bool comp_el(const element &a, const element &b) {
    //compare elements by value, also operators are 'bigger' than numbers
    //if (a < b) return true;
    if (a.type != b.type) return b.type == 𝕆;
    else return a.value < b.value;
}

bool comp(const expression &a, const expression &b) {
    //compare elements by value, also operators are 'bigger' than numbers
    //if (a < b) return true;
    if (a.empty() || b.empty())
        return false;

    auto[misA, misB] = std::mismatch(cbegin(a), cend(a), cbegin(b),
                                     [](const element &elA, const element &elB) {
                                         return elA.type == elB.type && elA.value == elB.value;
                                     }
    );

    return comp_el(*misA, *misB);
}


bool isValid(expression expr) {
    if (expr.empty()) return true;
    int n = 0;
    for (const auto &el: expr) {
        if (el.type == 𝕆) n--;
        else n++;
        if (n < 1) return false;
    }
    return n == 1;
}

bool isValidRng(expression::iterator a, expression::iterator b) {
    if (a == b) return true;
    int n = 0;
    for (auto el = a; el != b; ++el) {
        if (el->type == 𝕆) n--;
        else n++;
        if (n < 1) return false;
    }
    return n == 1;
}

void printPermutations(expression operators, int r){
    std::vector<expression> permutations;
    int n = 0, m = 0;

    for_each_permutation(begin(operators), begin(operators) + r, end(operators), [&permutations, &n, &m](auto a, auto b) {
        n++;
        if(n % 10'000'000 == 0) std::cout << "n: " << n / 10'000'000 << std::endl;
        if (isValidRng(a, b)) {
            m++;
            if(m % 100'000 == 0) std::cout << "m: "<< m << std::endl;
//            permutations.emplace_back(a, b);
//            std::copy(a, b, std::ostream_iterator<element>(std::cout, " "));
//            std::cout << " = " << eval(permutations.back()) << std::endl;
        }
        return false;
    });
    std::cout << "Total of " << n << ", valid " << m << std::endl;
//    std::cout << "Total of " << permutations.size() << std::endl;
}

int my_main() {

    expression operators{
            {𝕆, '+'},
            {𝕆, '-'},
            {𝕆, '/'},
            {𝕆, '*'},
            {𝕆, '*'},
            {ℤ,  1},
            {ℤ,  2},
            {ℤ,  3},
            {ℤ,  4},
            {ℤ,  5},
            {ℤ,  6}
    };

    auto t0 = std::chrono::steady_clock::now();

//    printPermutations(operators, 1);
//    printPermutations(operators, 3);
//    printPermutations(operators, 5);
//    printPermutations(operators, 7);
//    printPermutations(operators, 9);
    printPermutations(operators, 11);

    auto t1 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>{t1-t0}.count() << '\n';


    return 0;
}

#ifndef TEST

int main() { return my_main(); }

#endif

