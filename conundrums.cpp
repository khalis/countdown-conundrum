#include <regex>
#include <ranges>

#include "solver.h"
#include "printing.hpp"

void words_conundrum(Solver &solver, const std::string &conundrum) {
    std::vector<string> result = solver(conundrum);

    print("Found {} solutions", result.size());

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
            // auto start = now();
            // number_conundrum(line);
            // print("Done in {}\n", duration<double>(now()-start));
        }

    }
    return 0;
}