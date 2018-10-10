//
// Created by khalis on 28/09/18.
//

#include <vector>
#include <bitset>
#include <map>
#include <algorithm>
#include <stdexcept>

#include <fmt/core.h>
#include <fmt/format.h>

using namespace fmt::literals;

#include "utility.h"

/// In the expression 1 -- is a number, 0 is an operation
/// Expression is valid if
/// 1. length is one of 1, 3, 5, 7, 9, 11
/// 2. Number of '1's is more that number of '0's in any suffix
/// 3. Number of '1's is exactly one more that number of '0's for the whole expression

/// N -- length of the expression
/// L -- number of numbers
/// Z -- number of operators
/// L + Z = N and 2*Z + 1 = N

std::map<std::uint8_t, std::vector<expression_template>> generate_all_templates() {
    std::map<std::uint8_t, std::vector<expression_template>> templates{};
    size_t N = 1;
    for (expression_template tmpl = 1; N <= Longest_Expression; ++tmpl) {

        if (tmpl > (1u << (N - 1))) N += 2; /// Valid expression rule #1
        int result = 0;
        for (size_t i = 0; i < N; i++) {
            if ((tmpl & (1u << i)) != 0) result++;
            else result--;
            if (result < 1) goto reject_template; /// Valid expression rule #2
        }
        if (result == 1) templates[N].push_back(tmpl); /// Valid epression rule #3
        reject_template:;
    }
    return templates;
};

int main() {
    std::string prefix = "const map<std::uint8_t, vector<expression_template>> all_templates{";
    std::string middle = R"foo(
        //{key}
        {{{key}, {{ {templates} }} }})foo";
    std::string postfix = "};\n";

    auto all_templates = generate_all_templates();

    fmt::print("{}", prefix);

    for (auto key_value = begin(all_templates); key_value != end(all_templates); ++key_value) {
        auto &[key, templates] = *key_value;
        std::string templates_string{};
        for (auto tmpl = begin(templates); tmpl != end(templates); ++tmpl) {
            templates_string += fmt::format("{:#b}", *tmpl);
            if (next(tmpl) != end(templates)) templates_string += ", ";
        }
        fmt::print(middle, "key"_a = key, "templates"_a = templates_string);
        if (next(key_value) != end(all_templates)) fmt::print(",");
    }
    fmt::print("{}", postfix);
    return 0;
}