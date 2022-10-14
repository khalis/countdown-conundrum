//
// Created by khalis on 27-Jul-2022.
//
#include <vector>
#include <array>
#include <span>
#include <map>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <ostream>
#include <chrono>
#include <string>
#include <iostream>


#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <fmt/chrono.h>

#include "utility.h"
#include "combinations.h"
#include "iter_tools.h"
#include "operation.h"

using std::vector; using std::array; using std::span;
using std::accumulate; using std::transform;
using std::sort; using std::unique;
using std::next; using std::prev;
using fmt::print;

using std::chrono::steady_clock;

template <typename T>
    struct solution {
        size_t count;
        vector<T> numbers;
        vector<Operation> operations;
        vector<T> visited;
        solution(size_t count, span<T> numbers, span<Operation> operations, span<T> visited):
            count(count),
            numbers(begin(numbers), end(numbers)),
            operations(begin(operations), end(operations)),
            visited(begin(visited), end(visited))
         {}
};

template<typename T>
    std::string infix(vector<T> numbers, vector<Operation> operations);

template<typename T>
std::ostream &operator<<(std::ostream &os, const solution<T> &sol) {
    return os << fmt::format( "{}", infix(sol.numbers, sol.operations) );
}

// template<typename T>
// std::ostream &operator<<(std::ostream &os, const solution<T> &sol) {
//     return os << 
//         fmt::format( 
//             "{:3} {} {:>21} {} {}", 
//             // "{:3} {} {} {}", 
//             sol.count, sol.numbers, 
//             fmt::format("{}", sol.visited), 
//             sol.operations,
//             infix(sol.numbers, sol.operations)
//             );
// }

template <typename T> 
    bool valid_intermediate(T value)  {
        return true;
    }

template<> 
    bool valid_intermediate(rational<int> value) {
        return value.b==1 && value.a > 0;
    }

using frac = rational<int>;

template <typename T>
    vector<solution<T>> search(vector<T> numbers, std::span<Operation> operations, T goal){
        vector<solution<T>> solutions{};
        size_t count = 0;
        auto ops_num = numbers.size() - 1;
        vector<T> visited(numbers.size(), 0);
        for(const auto &ops: product<Operation, operations.size()>{operations, ops_num})
            for(const auto &nums: permut{numbers}){
                auto first_num = begin(nums);
                auto last_num = end(nums);
                auto first_op = begin(ops);
                auto result = accumulate(next(first_num), last_num, *first_num, 
                    [&first_op](const auto &a, const auto &b){
                        return applyOperation(a, b, *first_op++);
                    }
                );
                if (result != goal) continue;

                first_op = begin(ops);
                result = *first_num;
                auto visited_it = begin(visited);
                
                for(auto it = next(first_num); it != last_num; ++it, ++first_op){
                    if (*first_op == Operation::drop_right) continue;
                    auto new_result = applyOperation(result, *it, *first_op);
                    if (new_result == result || new_result == goal) continue;
                    result = new_result;
                    if (new_result == *it ) continue;
                    *visited_it++ = result;
                    if (!valid_intermediate(result)) goto next_permutation;
                }
                
                sort(begin(visited), visited_it);
                visited_it = unique(begin(visited), visited_it);
                solutions.push_back(solution<T>{count++, nums, ops, span{begin(visited), visited_it}});
                next_permutation:;
            }

        sort(begin(solutions), end(solutions), [](const auto &a, const auto &b){
            return a.visited < b.visited;
        });

        auto last = unique(begin(solutions), end(solutions), [](const auto &a, const auto &b){
            return a.visited == b.visited;
        });

        solutions.erase(last, end(solutions));
        return solutions;
    }


template<typename T>
    std::string infix(vector<T> numbers, vector<Operation> operations) {
        std::stack<std::pair<std::string, int>> stack;
        auto op = begin(operations);
        stack.push( {fmt::format("{}", numbers.front()), 0} );

        for(auto num = next(begin(numbers)); num!=end(numbers); ++num, ++op){
            if((*op == Operation::multiplication || *op == Operation::division)
                && *num == 1 ) continue;
            stack.push( {fmt::format("{}", *num), 0} );

            std::string result;
            auto[b, b_priority] = pop(stack);
            auto[a, a_priority] = pop(stack);
            int priority = [&op](){            
                switch (*op) {
                    case Operation::addition       : return 1;
                    case Operation::subtraction    : return 1;
                    case Operation::multiplication : return 2;
                    case Operation::division       : return 2;
                    case Operation::drop_right     : return 0;
                }}();

            result += (priority > a_priority && a_priority) ? "(" + a + ")" : a;
            if (*op != Operation::drop_right) {
                result += fmt::format(" {} ", *op);
                result += (priority > b_priority && b_priority) ? "(" + b + ")" : b;
            }
            if (*op == Operation::division) result = "(" + result + ")";

            stack.push({result, priority});
        }
        return stack.top().first;
    }



void run_search(std::string line){

    std::istringstream input{line};
    std::istream_iterator<int> first{input}, last;

    vector<rational<int>> numbers{first, last};
    auto goal = numbers.back();
    numbers.pop_back();


    // vector<rational<int>> numbers{25, 100, 1, 7, 9, 5};
    // auto goal = rational<int>(729);
    // constexpr auto operations = std::to_array(operations_list);

    auto start = steady_clock::now();
    auto solutions = search(numbers, operations_list, goal);
    auto done = steady_clock::now();
    
    print("Optimized in {:%S}s, left with {} solutions\n", done - start, solutions.size());
    print("{}\n", fmt::join(solutions, "\n"));
}


int main(){
    while (true) {
        std::string line;

        print("Enter puzzle: ");
        std::getline(std::cin, line);
        if (line.empty()) break;
        run_search(line);
    }

    return 0;
}