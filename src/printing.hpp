#pragma once

#include <iostream>
#include <format>
#include <string_view>
#include <string>


struct printing_format {
    const std::string prefix;
    const std::string delimiter;
    const std::string postfix;
};

template <typename... Args>
inline void print(std::string_view rt_fmt_str, Args&&... args) {
    std::cout << std::vformat(rt_fmt_str, std::make_format_args(args...));
}


template<typename T>
inline void print(T value){
    print("{}\n", value);
}

inline void print(){
    std::cout << std::endl;
}

inline std::string count(size_t number, const std::string& common, const std::string& singular, const std::string& plural){
    if(number == 1) return std::to_string(number) + common + singular;
    return std::to_string(number) + common + plural;
}

struct printer{
    using SV = std::string_view;
    printing_format format;
    bool first_use = true;

    printer(SV prefix, SV delimiter, SV postfix): 
        format{std::string(prefix), std::string(delimiter), std::string(postfix)} {}
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