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
    std::copy(begin(input), prev(end(input)), begin(numbers));

    if(std::ranges::count(numbers, goal) > 0)
        print("There is at least one trivial solution");

    auto solutions = search(numbers, goal);
    validate(solutions);
    optimize(solutions);
    winnow_solutions(solutions);
    if(solutions.size() == 0) {
        print("Did not find any solutions.");
        return;
    }
    print("Found {}\n", count(solutions.size(), " solution", ", here it is:", "s, here are 10:"));
    auto take_10 = std::ranges::views::take(10);
    for(auto &sol: solutions | take_10)
        print("{} = {}\n", sol.infix, goal);
}

int main() {
    using namespace std;
    Solver solver;

    while (true) {
        string line;

        print("Enter puzzle: ", 0);
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