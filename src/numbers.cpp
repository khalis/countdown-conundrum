//
// Created by khalis on 15/09/18.
//
#include <vector>
#include <string>
#include <regex>
#include <bitset>
#include <mutex>
#include <sstream>
#include <chrono>
#include <execution>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/chrono.h>
#include <iostream>

#include "combinations.h"
#include "utility.h"
#include "solver.h"

using std::vector;
using std::array;
using std::map;
using std::string;
using std::regex;
using std::bitset;
using std::size_t;


using fmt::print;

auto now = std::chrono::steady_clock::now;

/// In the expression 1 -- is a number, 0 is an operation
/// Expression is valid if
/// 1. length is one of 1, 3, 5, 7, 9, 11
/// 2. Number of '1's is more that number of '0's in any suffix
/// 3. Number of '1's is exactly one more that number of '0's for the whole expression


/// All valid expression templates
// clang-format off
const map<std::uint8_t, vector<expression_template>> all_templates{
        //1
        {1,  {0b1}},
        //3
        {3,  {0b011}},
        //5
        {5,  {0b00111,       0b01011}},
        //7
        {7,  {0b0010111,     0b0011011,
              0b0100111,     0b0101011}},
        //9
        {9,  {0b001001111,   0b001010111,
              0b001011011,   0b001100111,
              0b001101011,   0b010001111,
              0b010010111,   0b010011011,
              0b010100111,   0b010101011}},
        //11
        {11, {0b00100011111, 0b00100101111,
              0b00100110111, 0b00100111011,
              0b00101001111, 0b00101010111,
              0b00101011011, 0b00101100111,
              0b00101101011, 0b00110001111,
              0b00110010111, 0b00110011011,
              0b00110100111, 0b00110101011,
              0b01000011111, 0b01000101111,
              0b01000110111, 0b01000111011,
              0b01001001111, 0b01001010111,
              0b01001011011, 0b01001100111,
              0b01001101011, 0b01010001111,
              0b01010010111, 0b01010011011,
              0b01010100111, 0b01010101011}}
};
// clang-format on

/// N -- length of the expression
/// L -- number of numbers
/// Z -- number of operators
/// L + Z = N and 2*Z + 1 = N
vector<vector<int>> get_permutations(vector<int> &numbers, size_t L) {
    vector<vector<int>> permutations;
    auto first = begin(numbers);
    auto mid = first + L;
    auto last = end(numbers);
    for_each_permutation(first, mid, last,
                         [&permutations](auto a, auto z) {
                             permutations.emplace_back(a, z);
                             return false; //continue
                         });
    return permutations;
}

/// Enumerate all operator combinations
/// A selection of operators can be represented as a number in 4-base
/// To enumerate all combination we need to count from 0 to 4^Z-1,
/// where Z is how many operators required

vector<qint> get_operators(size_t Z) {
    size_t max = 1u << 2 * Z;
    vector<qint> result{max};
    for (size_t i = 0; i < max; ++i)
        result[i] = qint(static_cast<unsigned int>(i), Z);
    return result;
}

template<typename T>
vector<expression<T>> get_expressions(vector<T> &numbers, T goal) {
    vector<expression<T>> solutions;
    size_t count_combinations = 0;
    std::mutex m;
    for (const auto &[N, templates]: all_templates) {
        size_t L = (N + 1u) / 2u;
        if (numbers.size() < L) break;

        size_t Z = N - L;

        vector<qint> operators = get_operators(Z);
        vector<vector<int>> permutations = get_permutations(numbers, L);

        // print("templates {}, operators {}, permutations {}, {} \n\n", templates.size(), operators.size(), permutations.size(), L);

        for (const auto &tmpl: templates) {
            for (const auto &op: operators) {
                // std::for_each(/**/std::execution::par,/**/ begin(permutations), end(permutations), [&, N=N](const auto &num){
                for (const auto &num: permutations) 
                    {
                        // if(count_combinations++ % 225'831 == 0)
                            // move line up, erase line, move begging of the line
                            // print("\033[A\33[2K\r{:>3}%\n", count_combinations / 225'831);
                        size_t op_ind = 0;
                        size_t num_ind = 0;

                        expression<int> expr{N, tmpl, {}};

                        traverse(expr,
                                [&num_ind, &num](int &number) { number = static_cast<int>(num[num_ind++]); },
                                [&op_ind, &op](int &opr) { opr = static_cast<int>(op.digits[op_ind++].val); }
                        );

                        if (expr.isEqual(goal)) {
                            std::lock_guard guard(m);
                            solutions.push_back(expr);
                        }
                    }
                // });
            }
        }
    }
    return solutions;
}

void words_conundrum(Solver &solver, const string &conundrum) {
    vector<string> result = solver(conundrum);

    std::cout << "Found " << result.size() << " solutions\n";

    if (result.empty()) return;

    std::sort(begin(result), end(result),
              [](const string &a, const string &b) {
                  return a.length() > b.length();
              });

    auto length = result[0].length() + 1;
    size_t num = 0;

    for (const auto &word : result) {
        if (word.length() < length) {
            num++;
            if (num > 4) break;
            std::cout << "\n\n";
            std::cout << --length << "\n";
        }
        std::cout << word << " ";
    }
    std::cout << "\n";
}

void number_conundrum(const string &line) {
    int goal;

    std::istringstream input{line};
    std::istream_iterator<int> first{input}, last;

    vector<int> numbers{first, last};
    goal = numbers.back();
    numbers.pop_back();

    auto solutions = get_expressions(numbers, goal);
    if (solutions.empty())
        print("Found no solutions\n");
    else
        print("Found at least {} solutions\n", solutions.size());
    for (size_t i = 0; i != solutions.size(); ++i)
        print("{:2}) {} = {}\n",
              i + 1,
              solutions[i].eval(),
              solutions[i].infix()
        );
}

int main() {
    using std::chrono::duration;
    Solver solver;

    while (true) {
        string line;

        print("Enter puzzle: ");
        getline(std::cin, line);
        if (line.empty()) break;
        if (std::regex_match(line, regex{"[^[:digit:]]+"}))
            words_conundrum(solver, line);
        else{
            auto start = now();
            number_conundrum(line);
            print("Done in {}\n", duration<double>(now()-start));
        }

    }
    return 0;
}