//
// Created by khalis on 03/09/18.
//

#define TEST

#ifdef TEST
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "old version.cpp"

TEST_CASE("Expression evaluation", "[evaluation]") {
    REQUIRE(eval({}) == 0);
    REQUIRE(eval(""_expr) == 0);
    REQUIRE(eval("1 2 3 + +"_expr) == 6);
    REQUIRE(eval("1 5 3 - -"_expr) == -1);
    REQUIRE(eval("12 6 3 / /"_expr) == 6);
    REQUIRE(eval("2 3 *"_expr) == 6);
    REQUIRE(eval("10 2 / 3 *"_expr) == 15);
    REQUIRE(eval("10 2 + 3 *"_expr) == 36);
}

TEST_CASE("Expression validation", "[validation]") {

    REQUIRE(isValid("10 2 + 3 *"_expr) == true);
    REQUIRE(isValid("1 3 +"_expr) == true);
    REQUIRE(isValid(""_expr) == true);

    REQUIRE(isValid("+"_expr) == false);
    REQUIRE(isValid("1 +"_expr) == false);
    REQUIRE(isValid("1 2 + *"_expr) == false);
    REQUIRE(isValid("1 + 2 "_expr) == false);
    REQUIRE(isValid("+ 1 2 "_expr) == false);

    REQUIRE(isValid("1 2 3 + "_expr) == false);

}

TEST_CASE("Expression comparision", "[comparision]") {
    // ASCII codes of operators: (this will be sort order)
    // 47  /
    // 45  -
    // 43  +
    // 42  *
    REQUIRE(comp(""_expr, ""_expr) == false);
    REQUIRE(comp("*"_expr, "+"_expr) == true);
    REQUIRE(comp("+"_expr, "*"_expr) == false);
    REQUIRE(comp("*"_expr, "-"_expr) == true);
    REQUIRE(comp("*"_expr, "/"_expr) == true);
    REQUIRE(comp("+"_expr, "-"_expr) == true);
    REQUIRE(comp("/"_expr, "-"_expr) == false);

    REQUIRE(comp("*"_expr, "1"_expr) == false);

    REQUIRE(comp("1"_expr, "2"_expr) == true);
    REQUIRE(comp("1"_expr, "0"_expr) == false);
    REQUIRE(comp("1 2 *"_expr, "1 2 3"_expr) == false);
    REQUIRE(comp("1 1 *"_expr, "1 2 3"_expr) == true);
}

#endif
