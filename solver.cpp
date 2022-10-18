//
// Created by khalis on 09/09/18.
//

#include <algorithm>
#include <bitset>
#include <sstream>
#include "solver.h"
// #include "dictionary/dictionary.h"

// extern const char* dictionary;

hash_t hash(const string &str) {
    hash_t result = 0;
    for (auto ch: str) {
        if (ch == '-') ch = 26;
        else ch = 'z' - ch;
        result |= (1ull << static_cast<unsigned char>(ch));
    }
    return result;
}

inline size_t count(hash_t hash) {
    return std::bitset<BITS>( static_cast<hash_t>(hash)).count();
//    return __builtin_popcountll(hash);
}

inline bool is_contained(hash_t donor, hash_t recipient) {
    return count(donor | ~recipient) == BITS;
}


bool check(const string &conundrum, const string &candidate) {
    if (conundrum.length() < candidate.length()) return true;

    std::map<char, short> letters;
    for (const auto &ch : conundrum) letters[ch]++;

    for (const auto &ch : candidate)
        if (letters[ch] > 0) letters[ch]--;
        else return true;

    return false;
}

Solver::Solver() {
    const char* file_name = "../../words_alpha.txt";
    std::ifstream is{file_name};
    // std::istringstream is{dictionary};
    std::istream_iterator<string> first{is}, last;
    words.reserve(370'100);
    for (auto word = first; word != last; ++word) {
        words.push_back({hash(*word), *word});
    }
}

vector<string> Solver::operator()(const string &conundrum) {
    vector<string> result;
    hash_t conundrum_hash = hash(conundrum);

    for (const auto &word: words)
        if (is_contained(conundrum_hash, word.hash))
            result.push_back(word.text);

    auto new_end = std::remove_if(
            begin(result), end(result),
            [&conundrum](const string &candidate) {
                return check(conundrum, candidate);
            }
    );
    result.erase(new_end, end(result));
    return result;
}
