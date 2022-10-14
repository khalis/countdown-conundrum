#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <utility>
#include <iterator>
#include <chrono>
#include <algorithm>
#include <functional>
#include <optional>
#include <format>


#include "iter_tools.h"
#include "printing.hpp"

auto now = std::chrono::steady_clock::now;
using std::chrono::duration;

using namespace std::literals;
using std::array, std::vector, std::string, std::optional;
using std::cout, std::endl, std::format;
using std::distance, std::find,  std::prev;
using std::to_array;

namespace my {
    int add(int a, int b){ return a + b;}
    int sub(int a, int b){ return a - b;}
    int mul(int a, int b){ return a * b;}
    int div(int a, int b){ if (b==0) return 0; return a / b;}
}

using op = int(int, int);
array all_expressions = to_array<size_t>({0b011, 0b00111, 0b01011, 0b0001111, 0b0010111, 0b0011011, 0b0100111, 0b0101011, 0b000011111, 0b000101111, 0b000110111, 0b000111011, 0b001001111, 0b001010111, 0b001011011, 0b001100111, 0b001101011, 0b010001111, 0b010010111, 0b010011011, 0b010100111, 0b010101011, 0b00000111111, 0b00001011111, 0b00001101111, 0b00001110111, 0b00001111011, 0b00010011111, 0b00010101111, 0b00010110111, 0b00010111011, 0b00011001111, 0b00011010111, 0b00011011011, 0b00011100111, 0b00011101011, 0b00100011111, 0b00100101111, 0b00100110111, 0b00100111011, 0b00101001111, 0b00101010111, 0b00101011011, 0b00101100111, 0b00101101011, 0b00110001111, 0b00110010111, 0b00110011011, 0b00110100111, 0b00110101011, 0b01000011111, 0b01000101111, 0b01000110111, 0b01000111011, 0b01001001111, 0b01001010111, 0b01001011011, 0b01001100111, 0b01001101011, 0b01010001111, 0b01010010111, 0b01010011011, 0b01010100111, 0b01010101011});
size_t func(int a, int b, int c, int d, int e, int f, op op1, op op2, op op3, op op4, op op5, int goal){
	size_t res = 0;
	res |= static_cast<size_t>(goal == op1(a, b)) << 0; // 011
	res |= static_cast<size_t>(goal == op2(a, op1(b, c))) << 1; // 00111
	res |= static_cast<size_t>(goal == op2(op1(a, b), c)) << 2; // 01011
	res |= static_cast<size_t>(goal == op3(a, op2(b, op1(c, d)))) << 3; // 0001111
	res |= static_cast<size_t>(goal == op3(a, op2(op1(b, c), d))) << 4; // 0010111
	res |= static_cast<size_t>(goal == op3(op1(a, b), op2(c, d))) << 5; // 0011011
	res |= static_cast<size_t>(goal == op3(op2(a, op1(b, c)), d)) << 6; // 0100111
	res |= static_cast<size_t>(goal == op3(op2(op1(a, b), c), d)) << 7; // 0101011
	res |= static_cast<size_t>(goal == op4(a, op3(b, op2(c, op1(d, e))))) << 8; // 000011111
	res |= static_cast<size_t>(goal == op4(a, op3(b, op2(op1(c, d), e)))) << 9; // 000101111
	res |= static_cast<size_t>(goal == op4(a, op3(op1(b, c), op2(d, e)))) << 10; // 000110111
	res |= static_cast<size_t>(goal == op4(op1(a, b), op3(c, op2(d, e)))) << 11; // 000111011
	res |= static_cast<size_t>(goal == op4(a, op3(op2(b, op1(c, d)), e))) << 12; // 001001111
	res |= static_cast<size_t>(goal == op4(a, op3(op2(op1(b, c), d), e))) << 13; // 001010111
	res |= static_cast<size_t>(goal == op4(op1(a, b), op3(op2(c, d), e))) << 14; // 001011011
	res |= static_cast<size_t>(goal == op4(op2(a, op1(b, c)), op3(d, e))) << 15; // 001100111
	res |= static_cast<size_t>(goal == op4(op2(op1(a, b), c), op3(d, e))) << 16; // 001101011
	res |= static_cast<size_t>(goal == op4(op3(a, op2(b, op1(c, d))), e)) << 17; // 010001111
	res |= static_cast<size_t>(goal == op4(op3(a, op2(op1(b, c), d)), e)) << 18; // 010010111
	res |= static_cast<size_t>(goal == op4(op3(op1(a, b), op2(c, d)), e)) << 19; // 010011011
	res |= static_cast<size_t>(goal == op4(op3(op2(a, op1(b, c)), d), e)) << 20; // 010100111
	res |= static_cast<size_t>(goal == op4(op3(op2(op1(a, b), c), d), e)) << 21; // 010101011
	res |= static_cast<size_t>(goal == op5(a, op4(b, op3(c, op2(d, op1(e, f)))))) << 22; // 00000111111
	res |= static_cast<size_t>(goal == op5(a, op4(b, op3(c, op2(op1(d, e), f))))) << 23; // 00001011111
	res |= static_cast<size_t>(goal == op5(a, op4(b, op3(op1(c, d), op2(e, f))))) << 24; // 00001101111
	res |= static_cast<size_t>(goal == op5(a, op4(op1(b, c), op3(d, op2(e, f))))) << 25; // 00001110111
	res |= static_cast<size_t>(goal == op5(op1(a, b), op4(c, op3(d, op2(e, f))))) << 26; // 00001111011
	res |= static_cast<size_t>(goal == op5(a, op4(b, op3(op2(c, op1(d, e)), f)))) << 27; // 00010011111
	res |= static_cast<size_t>(goal == op5(a, op4(b, op3(op2(op1(c, d), e), f)))) << 28; // 00010101111
	res |= static_cast<size_t>(goal == op5(a, op4(op1(b, c), op3(op2(d, e), f)))) << 29; // 00010110111
	res |= static_cast<size_t>(goal == op5(op1(a, b), op4(c, op3(op2(d, e), f)))) << 30; // 00010111011
	res |= static_cast<size_t>(goal == op5(a, op4(op2(b, op1(c, d)), op3(e, f)))) << 31; // 00011001111
	res |= static_cast<size_t>(goal == op5(a, op4(op2(op1(b, c), d), op3(e, f)))) << 32; // 00011010111
	res |= static_cast<size_t>(goal == op5(op1(a, b), op4(op2(c, d), op3(e, f)))) << 33; // 00011011011
	res |= static_cast<size_t>(goal == op5(op2(a, op1(b, c)), op4(d, op3(e, f)))) << 34; // 00011100111
	res |= static_cast<size_t>(goal == op5(op2(op1(a, b), c), op4(d, op3(e, f)))) << 35; // 00011101011
	res |= static_cast<size_t>(goal == op5(a, op4(op3(b, op2(c, op1(d, e))), f))) << 36; // 00100011111
	res |= static_cast<size_t>(goal == op5(a, op4(op3(b, op2(op1(c, d), e)), f))) << 37; // 00100101111
	res |= static_cast<size_t>(goal == op5(a, op4(op3(op1(b, c), op2(d, e)), f))) << 38; // 00100110111
	res |= static_cast<size_t>(goal == op5(op1(a, b), op4(op3(c, op2(d, e)), f))) << 39; // 00100111011
	res |= static_cast<size_t>(goal == op5(a, op4(op3(op2(b, op1(c, d)), e), f))) << 40; // 00101001111
	res |= static_cast<size_t>(goal == op5(a, op4(op3(op2(op1(b, c), d), e), f))) << 41; // 00101010111
	res |= static_cast<size_t>(goal == op5(op1(a, b), op4(op3(op2(c, d), e), f))) << 42; // 00101011011
	res |= static_cast<size_t>(goal == op5(op2(a, op1(b, c)), op4(op3(d, e), f))) << 43; // 00101100111
	res |= static_cast<size_t>(goal == op5(op2(op1(a, b), c), op4(op3(d, e), f))) << 44; // 00101101011
	res |= static_cast<size_t>(goal == op5(op3(a, op2(b, op1(c, d))), op4(e, f))) << 45; // 00110001111
	res |= static_cast<size_t>(goal == op5(op3(a, op2(op1(b, c), d)), op4(e, f))) << 46; // 00110010111
	res |= static_cast<size_t>(goal == op5(op3(op1(a, b), op2(c, d)), op4(e, f))) << 47; // 00110011011
	res |= static_cast<size_t>(goal == op5(op3(op2(a, op1(b, c)), d), op4(e, f))) << 48; // 00110100111
	res |= static_cast<size_t>(goal == op5(op3(op2(op1(a, b), c), d), op4(e, f))) << 49; // 00110101011
	res |= static_cast<size_t>(goal == op5(op4(a, op3(b, op2(c, op1(d, e)))), f)) << 50; // 01000011111
	res |= static_cast<size_t>(goal == op5(op4(a, op3(b, op2(op1(c, d), e))), f)) << 51; // 01000101111
	res |= static_cast<size_t>(goal == op5(op4(a, op3(op1(b, c), op2(d, e))), f)) << 52; // 01000110111
	res |= static_cast<size_t>(goal == op5(op4(op1(a, b), op3(c, op2(d, e))), f)) << 53; // 01000111011
	res |= static_cast<size_t>(goal == op5(op4(a, op3(op2(b, op1(c, d)), e)), f)) << 54; // 01001001111
	res |= static_cast<size_t>(goal == op5(op4(a, op3(op2(op1(b, c), d), e)), f)) << 55; // 01001010111
	res |= static_cast<size_t>(goal == op5(op4(op1(a, b), op3(op2(c, d), e)), f)) << 56; // 01001011011
	res |= static_cast<size_t>(goal == op5(op4(op2(a, op1(b, c)), op3(d, e)), f)) << 57; // 01001100111
	res |= static_cast<size_t>(goal == op5(op4(op2(op1(a, b), c), op3(d, e)), f)) << 58; // 01001101011
	res |= static_cast<size_t>(goal == op5(op4(op3(a, op2(b, op1(c, d))), e), f)) << 59; // 01010001111
	res |= static_cast<size_t>(goal == op5(op4(op3(a, op2(op1(b, c), d)), e), f)) << 60; // 01010010111
	res |= static_cast<size_t>(goal == op5(op4(op3(op1(a, b), op2(c, d)), e), f)) << 61; // 01010011011
	res |= static_cast<size_t>(goal == op5(op4(op3(op2(a, op1(b, c)), d), e), f)) << 62; // 01010100111
	res |= static_cast<size_t>(goal == op5(op4(op3(op2(op1(a, b), c), d), e), f)) << 63; // 01010101011
	return res;
}

template<typename T>
auto double_pop(T &vec) -> std::pair<typename T::value_type, typename T::value_type> {
    auto a = vec.back(); vec.pop_back();
    auto b = vec.back(); vec.pop_back();
    return std::make_pair(a, b);
}
struct Solution{
    size_t ind;
    size_t expression;
    array<int, 6> numbers;
    array<size_t, 5> operations;
    string infix;
};

vector<Solution> search(array<int, 6> numbers, int goal){
    vector<Solution> solutions;
    array ops {*my::add, *my::sub, *my::mul, *my::div};
    auto ops_ind = to_array<size_t>({0, 1, 2, 3});
    size_t ind = 0;
    // for(auto [op1, op2, op3, op4, op5]: product{ops_ind, 5})
    for(auto op1: ops_ind)
     for(auto op2: ops_ind)
      for(auto op3: ops_ind)
       for(auto op4: ops_ind)
        for(auto op5: ops_ind)
            for(auto [a, b, c, d, e, f]: permut{numbers})
                if(auto res = func(a,b,c,d,e,f, ops[op1],ops[op2],ops[op3],ops[op4],ops[op5], goal); res !=0)
                    for(size_t i=0; i < all_expressions.size(); ++i)
                        if(res & (1ull << i))
                            solutions.push_back({ind++, all_expressions[i], {a, b, c, d, e, f}, {op1, op2, op3, op4, op5}, ""});
            
    return solutions;
}

struct Operation { char as_char; int priority; std::optional<int> (*function)(int,int); };

constexpr auto operations = std::to_array<Operation>({
        {'+', 1, [] (int a, int b) -> std::optional<int> { return a + b; } },
        {'-', 1, [] (int a, int b) -> std::optional<int> { return a - b; } },
        {'*', 2, [] (int a, int b) -> std::optional<int> { return a * b; } },
        {'/', 2, [] (int a, int b) -> std::optional<int> { if (!b || (a % b)) return std::nullopt; return a / b;} }
    });

struct Stack_element {
    int value;
    int priority;
    string as_string;
};

vector<Solution> validate(vector<Solution> &solutions){
    auto new_end = std::remove_if(begin(solutions), end(solutions), [](Solution &solution){
        
        auto ops_it  = begin(solution.operations);
        auto last_op = end(solution.operations);
        auto nums_it = begin(solution.numbers);

        vector<Stack_element> stack;

        for(size_t i=0; ops_it != last_op; ++i){
            if(solution.expression & (1ull << i)) {
                stack.push_back({*nums_it, 0, format("{}", *nums_it)});
                ++nums_it;
                continue;
            }
            if (stack.size() < 2) break;
            string result;
            Operation operation = operations[*ops_it++];
            auto [b, a] = double_pop(stack);

            auto res = operation.function(a.value, b.value);
            if(!res) return true; // function evaluation wasn't valid: eg div by zero, or float div

            if(operation.as_char == '+' || operation.as_char == '*')
                if (a.value < b.value) std::swap(a, b);
            
            if(operation.as_char == '*' && b.value == 1) return true;
            if(operation.as_char == '/' && b.value == 1) return true;

            // auto pack = [](auto pr, auto pra, auto a){ return (pr != pra && pra) ? format("({})", a) : a; };
            auto pack = [](auto pr, auto pra, auto a){ return pra? format("({})", a) : a; };
            result = format("{}{}{}", 
                pack(operation.priority, a.priority, a.as_string), 
                operation.as_char, 
                pack(operation.priority, b.priority, b.as_string)
            );

            stack.push_back( {res.value(), operation.priority, result} );
        }
        solution.infix = stack.back().as_string;
        return false;
    });
    solutions.erase(new_end, end(solutions));
    return solutions;
}

vector<Solution> optimize(vector<Solution> &solutions){
    std::sort(begin(solutions), end(solutions), [](const auto &a, const auto &b){
            return a.infix < b.infix;
        });
    auto new_end = std::unique(begin(solutions), end(solutions), [](const auto &a, const auto &b){
            return a.infix == b.infix;
        });
    solutions.erase(new_end, end(solutions));
    return solutions;
}

vector<Solution> unique_expressions(vector<Solution>& solutions){

    return solutions;
}


void run(array<int, 7> input, bool verbose = false){

    array<int, 6> numbers;
    auto goal = input.back();
    copy(begin(input), prev(end(input)), begin(numbers));

    sort(begin(numbers), end(numbers), std::greater{});

    for(auto prn = printer("=====\n", " ", std::format(" = {}\n", goal)); auto n : numbers) prn(n);

    if(auto res = std::find(begin(numbers), end(numbers), goal); res != end(numbers))
        print("There is at least one trivial solution, {}", *res);


    auto start = now();
    auto solutions = search(numbers, goal);
    print("In {} found {} candidates\n", duration<double>(now() - start), solutions.size() );
    validate(solutions);
    print("After validation left with {} solutions\n", solutions.size() );
    optimize(solutions);
    print("After optimization left with {} solutions\n", solutions.size() );
    // std::cin.get();
    if (verbose){
        for(auto &sol: solutions) {
            print("{}, 0b{:b}, ", sol.ind, sol.expression);
            for(auto prn = printer("{",", ","}, "); auto i: sol.numbers) prn(i);
            for(auto prn = printer("{",", ","}, "); auto op: sol.operations) prn(op);
            print("\"{}\"\n", sol.infix);
        }

    }
    else for(auto &sol: solutions) print("{}) {}={}\n", sol.ind, sol.infix, goal);
}

int main (int argc, char *argv[]){
    using namespace std;
    if(argc > 1 && argv[1] == "test"sv){
        auto some_cases = to_array<array<int, 7>> ({
            // {1, 3, 7, 8, 75, 100, 998},
            // {50, 100, 75, 25, 2, 9, 327},
            // {1, 7, 9, 5, 25, 100, 729},
            {5, 5, 8, 10, 75, 100, 844},
            // {5, 5, 8, 10, 75, 100, 100}, // finds a lot of solutions and candidates
            {50, 100, 6, 7, 6, 8, 981} });
        for(auto &test: some_cases)
            run(test, true);
        return 0;
    }

    while (true) {
        string line;

        cout << ("Enter puzzle: ");
        getline(cin, line);
        if (line.empty()) break;
        istringstream input{line};
        istream_iterator<int> first{input};
        array<int, 7> numbers;
        copy_n(first, 7, begin(numbers));
        run(numbers);
    }
    
    return 0;
}
