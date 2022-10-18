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
#include "utility.h"
#include "fast_num.h"
#include "generated.h"


static auto now = std::chrono::steady_clock::now;
using std::chrono::duration;

using namespace std::literals;
using std::array, std::vector, std::string, std::optional;
using std::cout, std::endl, std::format;
using std::distance, std::find,  std::prev;
using std::to_array;

namespace fast_ops {
    int add(int a, int b){ return a + b;}
    int sub(int a, int b){ return a - b;}
    int mul(int a, int b){ return a * b;}
    int div(int a, int b){ if (b==0) return 0; return a / b;}
}

constexpr operation* ops_array[] = {
    *fast_ops::add,
    *fast_ops::sub,
    *fast_ops::mul,
    *fast_ops::div
};

vector<Solution> search(array<int, 6> numbers, int goal){
    vector<Solution> solutions;
    size_t ind = 0;
    // for(auto [op1, op2, op3, op4, op5]: product{ops_ind, 5})
    for(size_t op1: {0, 1, 2, 3})
     for(size_t op2: {0, 1, 2, 3})
      for(size_t op3: {0, 1, 2, 3})
       for(size_t op4: {0, 1, 2, 3})
        for(size_t op5: {0, 1, 2, 3})
            for(auto [a, b, c, d, e, f]: permut{numbers})
                if(auto res = func(a,b,c,d,e,f, ops_array[op1],ops_array[op2],ops_array[op3],ops_array[op4],ops_array[op5], goal); res !=0)
                    for(size_t i=0; i < all_expressions.size(); ++i)
                        if(res & (1ull << i))
                            solutions.push_back({ind++, all_expressions[i], {a, b, c, d, e, f}, {op1, op2, op3, op4, op5}, ""});
            
    return solutions;
}



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

int main1 (int argc, char *argv[]){
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
