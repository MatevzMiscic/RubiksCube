#include "Combination.h"




std::array<std::array<short, 14>, 13> bin = {{}};

void cmb::calcbin(){
    bin = {{}};
    for(int n = 0; n < 13; ++n){
        bin[n][0] = 1;
        for(int k = 1; k < n; ++k){
            bin[n][k] = bin[n-1][k-1] + bin[n-1][k];
        }
        bin[n][n] = 1;
    }
}

int cmb::rank(int n, int k, int comb){
    int rank = 0;
    int count = 0;
    for(int i = 0; i < n; ++i){
        if(comb & (1 << i)){
            count += 1;
            rank += bin[i][count];
            //printf("i = %d, count = %d\n", i, count);
        }
    }
    return rank;
}
