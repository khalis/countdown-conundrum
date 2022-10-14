//
// Created by khalis on 28-Jul-2022.
//

#include <vector>
#include <span>
#include <iterator>
#include <algorithm>

/// Callback style product function

// similar to python's itertools.prod( [first..last), repeat=N)
// does not create new lists, but writes contiguously starting at out_first for each
template <typename Iterator, typename Iterator2, typename Function>
    void for_each_prod(Iterator first, Iterator last, Iterator2 out_first, size_t N, Function f){
        auto base = std::distance(first, last);
        auto max = ipow(base, N);
        std::fill(out_first, out_first + N, *first);
        for(size_t i = 0; i < max; i++){
            auto counter = i;
            auto it = out_first;
            while(counter){
                *it = first[counter % base];
                it++;
                counter /= base;
            }
            f(out_first, out_first + N);
        }
    }

///
/// Product function for range-based for loop
/// Algorithm uses counting in B-based number system, where B number of elements in input list
/// 

constexpr size_t ipow(size_t base, size_t exp){
    size_t result = 1;
    for (;;){
        if (exp & 1) result *= base;
        exp >>= 1;
        if (!exp) return result;
        base *= base;
    }
}


struct last_product {};

template<typename T, int Base>
    struct product{
        std::array<T, Base> list;
        std::array<T, 5> result;
        size_t ind;
        size_t max_ind;
        size_t base;
        product(const std::array<T, Base> &list, size_t repeat)
            : list{list}, ind{0}{
            result.fill(list.front());
            base = list.size();
            max_ind = ipow(base, repeat);
        }
        product operator++(){
            ++ind;
            return *this;
        }
        auto operator* () { 
            auto counter = ind;
            auto it = begin(result);
            while(counter){
                *it = list[counter % base];
                it++;
                counter /= base;
            }
            return result;
        }

        template <typename S, int Base2>
            friend bool operator==(const product<S, Base2> &pr, last_product){
                return pr.ind == pr.max_ind;
            }
    };

template<typename T, size_t Base> product(std::array<T, Base>, size_t ) 
    -> product<T,Base>;


template <typename T, int Base>
    product<T, Base> begin(product<T, Base> &pr) { return pr; }

template <typename T, int Base>
    last_product end(const product<T, Base> &pr) { return last_product{}; }

///
/// Permutaions generating function for range-based for loop
/// https://en.wikipedia.org/wiki/Heap%27s_algorithm non-recursive version
///

struct last_permut {};

template <typename T, size_t N>
    struct permut {
        std::array<T, N> result;
        std::array<size_t, N> c;
        size_t i;

        permut(const std::array<T, N> &list): result{list}, i{1} { c.fill(0); }

        permut operator++(){
            while (i < N && c[i] >= i ) {
                c[i] = 0;
                i++;
            }
            if (i==N) return *this;

            if (i % 2 == 0) std::swap(result[0], result[i]);
            else  std::swap(result[c[i]], result[i]);
            c[i]++;
            i = 1;
            return *this;
        }

        auto operator* () { 
            return result;
        }

        template <typename S, size_t N1>
            friend bool operator==(const permut<S, N1> &pr, last_permut){
                return (pr.i == N1);
            }
    };

template <typename T, size_t N> 
    permut(std::array<T, N>) -> permut<T, N>;


template <typename T, size_t N>
    permut<T, N> begin(permut<T, N> &pr) { return pr; }

template <typename T, size_t N>
    last_permut end(const permut<T, N> &pr) { return last_permut{}; }
