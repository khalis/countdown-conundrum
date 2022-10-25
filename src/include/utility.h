//
// Created by khalis on 28/09/18.
//
#pragma once

#include <array>
#include <string>
#include <utility>

struct Solution{
    size_t ind;
    size_t expression;
    std::array<int, 6> numbers;
    std::array<size_t, 5> operations;
    std::string infix;
};


template<typename T>
auto double_pop(T &vec) -> std::pair<typename T::value_type, typename T::value_type> {
    auto a = vec.back(); vec.pop_back();
    auto b = vec.back(); vec.pop_back();
    return std::make_pair(a, b);
}