#pragma once

#include <iostream>
#include <format>
#include <string_view>


struct printing_format {
    const std::string_view prefix;
    const std::string_view delimiter;
    const std::string_view postfix;
};

template <typename... Args>
void print(std::string_view rt_fmt_str, Args&&... args) {
    std::cout << std::vformat(rt_fmt_str, std::make_format_args(args...));
}


template<typename T>
void print(T value){
    print("{}\n", value);
}

void print(){
    std::cout << std::endl;
}

struct printer{
    using SV = std::string_view;
    printing_format format;
    bool first_use = true;

    printer(SV prefix, SV delimiter, SV postfix): format{prefix, delimiter, postfix} {}
    ~printer(){ std::cout << format.postfix; }

    template <typename... Args>
    void printing_impl(SV rt_fmt_str, Args&&... args) {
        if(first_use){
            first_use = false;
            std::cout << format.prefix;
        }
        else std::cout << format.delimiter;
        std::cout << std::vformat(rt_fmt_str, std::make_format_args(args...));
    }

    template<typename T>
    void operator()(T&& value) { printing_impl("{}", std::forward<T>(value)); }

    template <typename... Args>
    void operator()(SV rt_fmt_str, Args&&... args) {
        printing_impl(rt_fmt_str, std::forward<Args...>(args...));
    }

    void operator()(){
        std::cout << std::endl;
    }
};