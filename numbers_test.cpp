//
// Created by khalis on 28/09/18.
//


#define TEST

#ifdef TEST
#define CATCH_CONFIG_MAIN

#include "catch.hpp"

//#include "numbers.cpp"
#include "utility.h"

TEST_CASE("Rational class tests", "[rational]") {
    rational<int> a{1, 2};
    REQUIRE(a + a == 1);
}


#endif

