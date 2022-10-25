//
// Created by khalis on 09/09/18.
//

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>

#ifndef COUNTDOWN_WORDS_SOLVER_H
#define COUNTDOWN_WORDS_SOLVER_H

using std::string;
using std::vector;

using hash_t = unsigned long long;

constexpr auto BITS = std::numeric_limits<hash_t>::digits;

struct Word {
    hash_t hash;
    string text;
};

hash_t hash(const string &str);

inline size_t count(hash_t hash);

/***
* Checks that word with hash _donor_ has all the letters to make a word with hash _recipient_
* (a | ~b) part is boolean function #11
* a b a|~b
* 0 0  1
* 0 1  0
* 1 0  1
* 1 1  1
* if all bits are 1, then the condition is satisfied
* this does not take into account repeating letters
*/

inline bool is_contained(hash_t donor, hash_t recipient);

//More precise check that candidate word can be constructed from letters of the conundrum
bool check(const string &conundrum, const string &candidate);

struct Solver {
    vector<Word> words;

    Solver();
    vector<string> operator() (const string& conundrum);
};


#endif //COUNTDOWN_WORDS_SOLVER_H
