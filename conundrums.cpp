#include <regex>
#include <ranges>
#include <array>
#include <vector>
#include <chrono>
#include <iterator>

#include "solver.h"
#include "printing.hpp"
#include "fast_num.h"
#include "optimizer.h"

static auto now = std::chrono::steady_clock::now;
using std::chrono::duration;

void words_conundrum(Solver &solver, const std::string &conundrum) {
    std::vector<string> result = solver(conundrum);

    print("Found {}", count(result.size(), " solution", "", "s"));

    if (result.empty()) return;

    std::ranges::sort(result, std::greater{}, &std::string::length);

    auto length = result[0].length() + 1;
    size_t num = 0;

    for (const auto& word : result) {
        if (word.length() < length) {
            num++;
            if (num > 4) break;
            print("\n\n{}\n", --length);
        }
        print("{} ", word);
    }
    std::cout << std::endl;
}

void numbers_conundrum(std::array<int, 7> input, bool verbose = false){

    std::array<int, 6> numbers;
    auto goal = input.back();
    copy(begin(input), prev(end(input)), begin(numbers));

    sort(begin(numbers), end(numbers), std::greater{});

    for(auto prn = printer("=====\n", " ", std::format(" = {}\n", goal)); auto n : numbers) prn(n);

    if(auto res = std::find(begin(numbers), end(numbers), goal); res != end(numbers))
        print("There is at least one trivial solution, {}", *res);


    auto start = now();
    auto solutions = search(numbers, goal);
    print("In {} found {}\n", duration<double>(now() - start), count(solutions.size(), " candidate", "", "s"));
    validate(solutions);
    print("After validation left with {}\n", count(solutions.size(), " candidate", "", "s"));
    optimize(solutions);
    print("After optimization left with {}\n", count(solutions.size(), " candidate", "", "s"));
    winnow_solutions(solutions);
    print("After winnowing left with {}\n", count(solutions.size(), " candidate", "", "s"));
    // std::cin.get();
    std::ranges::sort(solutions, {}, &Solution::ind);
    if (verbose){
        for(auto &sol: solutions) {
            print("{}, 0b{:b}, ", sol.ind, sol.expression);
            for(auto prn = printer("{",", ","}, "); auto i: sol.numbers) prn(i);
            for(auto prn = printer("{",", ","}, "); auto op: sol.operations) prn(op);
            print("\"{}\"\n", sol.infix);
        }

    }
    else for(auto &sol: solutions) print("{: >4}) {}={}\n", sol.ind, sol.infix, goal);
}

int main() {
//     using std::chrono::duration;
    using namespace std;
    Solver solver;

    while (true) {
        string line;

        print("Enter puzzle: ");
        getline(std::cin, line);
        if (line.empty()) break;
        if (regex_match(line, regex{"[^[:digit:]]+"}))
            words_conundrum(solver, line);
        else{
            std::istringstream input{line};
            std::istream_iterator<int> first{input};
            std::array<int, 7> numbers;
            copy_n(first, 7, begin(numbers));
            numbers_conundrum(numbers);
        }

    }
    return 0;
}