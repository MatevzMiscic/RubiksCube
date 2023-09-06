#pragma once

#include <array>

namespace cmb{

    // fills bin with binomial coefficients
    void calcbin();

    // computes rank of a combination
    int rank(int n, int k, int comb);

}