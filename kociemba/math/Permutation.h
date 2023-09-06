#pragma once

#include <array>
#include "Set.h"


namespace perm{
    static std::array<int, 13> fact = {0, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

    // returns the rank of the perm[i,...,n-1] on set
    template<size_t n>
    int rank_helper(const std::array<byte, n>& perm, int i, Set& set){
        if(i >= n - 1) return 0;
        int a = set.rank(perm[i]);
        set.del(perm[i]);
        int b = rank_helper(perm, i + 1, set);
        return a * fact[n - 1 - i] + b;
    }

    // returns the lexicographic rank of the given permutation of integers 0, 1,...,n-1
    template<size_t n>
    int rank(const std::array<byte, n>& perm){
        Set set(n);
        return rank_helper(perm, 0, set);
    }

    // returns product of permutations a and b where a is applied first
    template<size_t n>
    std::array<byte, n> mul(std::array<byte, n>& a, std::array<byte, n>& b){
        std::array<byte, n> c;
        for(int i = 0; i < n; ++i) c[i] = b[a[i]];
        return c;
    }
}