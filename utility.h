//
// Created by khalis on 28/09/18.
//

#include <vector>
#include <array>
#include <stack>
#include <numeric>
#include <tuple>
#include <functional>
#include <ostream>

#include <fmt/format.h>

#ifndef COUNTDOWN_GAME_UTILITY_H
#define COUNTDOWN_GAME_UTILITY_H


/// stackoverflow.com/q/101439/
size_t ipow(size_t base, size_t exp){
    size_t result = 1;
    for (;;){
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) return result;
        base *= base;
    }
}


const size_t Longest_Expression = 11;
constexpr size_t Max_Operators = (Longest_Expression - 1) / 2;

constexpr char operators_to_char[] = "+-*/";

using expression_template = std::uint16_t;


template<typename T>
struct rational {
    T a, b;

    rational(): a{}, b{}{
    }

    rational(T num, T denum = 1) {
        T gcd = std::gcd(num, denum);
        a = num / gcd;
        b = denum / gcd;
    }

    rational<T> operator+(const rational<T> &rhs) const {
        const auto&[c, d] = rhs;
        return rational{a * d + c * b, b * d};
    }

    rational<T> operator-(const rational<T> &rhs) const {
        return *this + rational<T>{-rhs.a, rhs.b};
    }

    rational<T> operator*(const rational<T> &rhs) const {
        const auto&[c, d] = rhs;
        return rational<T>{a * c, b * d};
    }

    rational<T> operator/(const rational<T> &rhs) const {
        const auto&[c, d] = rhs;
        return rational<T>{a * d, b * c};
    }

    bool operator==(const rational<T> &rhs) const {
        const auto&[c, d] = rhs;
        return a == c && b == d;
    }

    bool operator==(T rhs) const {
        return a == rhs && b == 1;
    }

    bool operator <(const rational<T> &rhs) const {
        const auto&[c, d] = rhs;
        T lcm = std::lcm(b, d);
        return a*lcm < c*lcm;
    }

};

template<typename T>
std::ostream &operator<<(std::ostream &os, const rational<T> &fr) {
    if (fr.b == 1) return os << fr.a;
    return os << fr.a << "/" << fr.b;
}

union Digit {
    unsigned char val: 2;
};

std::ostream &operator<<(std::ostream &os, const Digit &digit) {
    return os << operators_to_char[digit.val];
}

struct qint {
    size_t width{Max_Operators};
    std::array<Digit, Max_Operators> digits{};

    qint() { digits.fill({0}); }

    explicit qint(unsigned int n, size_t width = Max_Operators) : qint() {
        this->width = width;
        constexpr unsigned int base = 4;
        for (size_t ind = 0; n; ++ind) {
            digits[ind].val = static_cast<unsigned char>( n % base);
            n /= base;
        }
    }
};

std::ostream &operator<<(std::ostream &os, const qint &number) {
    for (size_t i = 0; i < number.width; i++)
        os << number.digits[i];
    return os;
}

template<typename T>
T pop(std::stack<T> &stack) {
    auto element = stack.top();
    stack.pop();
    return element;
}

template<typename T>
struct expression {
    int width;
    expression_template tmpl;
    std::array<T, Longest_Expression> expr;


    bool isEqual(T goal) {
        std::stack<rational<T>> stack;
        bool always_whole = true;
        std::map<
                std::uint8_t,
                std::function<rational<T>(rational<T>, rational<T>)>
        > operators{
                {0, [](auto a, auto b) { return a + b; }},
                {1, [](auto a, auto b) { return a - b; }},
                {2, [](auto a, auto b) { return a * b; }},
                {3, [](auto a, auto b) { return a / b; }}
        };
        traverse(*this,
                 [&stack](int num) { stack.push(rational<T>{num, 1}); },
                 [&stack, &operators, &always_whole](int op) {
                     auto a = pop(stack);
                     auto b = pop(stack);
                     stack.push(operators[op](a, b));
                     always_whole = always_whole && (stack.top().b == 1);
                 }
        );
        return (stack.top() == goal) && always_whole;
    }

    rational<T> eval() {
        std::stack<rational<T>> stack;
        std::map<
                std::uint8_t,
                std::function<rational<T>(rational<T>, rational<T>)>
        > operators{
                {0, [](auto a, auto b) { return a + b; }},
                {1, [](auto a, auto b) { return a - b; }},
                {2, [](auto a, auto b) { return a * b; }},
                {3, [](auto a, auto b) { return a / b; }}
        };
        traverse(*this,
                 [&stack](int num) { stack.push(rational<T>{num, 1}); },
                 [&stack, &operators](int op) {
                     auto a = pop(stack);
                     auto b = pop(stack);
                     stack.push(operators[op](a, b));
                 }
        );
        return stack.top();
    }

    std::string infix() {
        std::stack<std::pair<std::string, int>> stack;
        traverse(*this,
                 [&stack](int num) { stack.push( {fmt::format("{}", num), 0} ); },
                 [&stack](int op) {
                     std::string result;
                     auto[b, b_priority] = pop(stack);
                     auto[a, a_priority] = pop(stack);
                     int priority = op / 2 + 1;
                     result += (priority > b_priority && b_priority) ? "(" + b + ")" : b;
                     result += operators_to_char[op];
                     result += (priority > a_priority && a_priority) ? "(" + a + ")" : a;
                     if (operators_to_char[op] == '/') result = "(" + result + ")";

                     stack.push({result, priority});

                 }
        );
        return stack.top().first;
    }

};

inline bool get_bit(expression_template tmpl, size_t index) {
    return static_cast<bool>(tmpl & (1u << index));
}

template<typename T>
void traverse(expression<T> &expr,
              const std::function<void(int &)> &number_case,
              const std::function<void(int &)> &operator_case) {
    for (size_t i = 0; i < expr.width; ++i)
        if (get_bit(expr.tmpl, i))
            number_case(expr.expr[i]);
        else
            operator_case(expr.expr[i]);
}

template<typename T>
void traverse(const expression<T> &expr,
              const std::function<void(int)> &number_case,
              const std::function<void(int)> &operator_case) {
    for (size_t i = 0; i < expr.width; ++i)
        if (get_bit(expr.tmpl, i))
            number_case(expr.expr[i]);
        else
            operator_case(expr.expr[i]);
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const expression<T> &expr) {
    traverse(expr,
             [&os](int num) { os << " " << num << " "; },
             [&os](int op) { os << " " << operators_to_char[op] << " "; }
    );
    return os;
}

#endif //COUNTDOWN_GAME_UTILITY_H
