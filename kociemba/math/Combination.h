#pragma once

#include <array>

namespace cmb{

    std::array<std::array<short, 14>, 13> bin = {{}};

    void calcbin(){
        for(int n = 0; n < 13; ++n){
            bin[n][0] = 1;
            for(int k = 1; k < n; ++k){
                bin[n][k] = bin[n-1][k-1] + bin[n-1][k];
            }
            bin[n][n] = 1;
        }
    }
    /*
    int rank(int n, int k, int comb){
        int rank = 0;
        int i = 0;
        while((1 << (n - i)) - 1 >= comb) i += 1;
        printf("i = %d\n", i);
        int count = 0;
        for(; i < n; ++i){
            if(comb & (1 << (n - i - 1))){
                count += 1;
            }else{
                rank += bin[i][count];
                printf("i = %d, count = %d\n", i, count);
            }
        }
        return rank;
    }
    */
    int rank(int n, int k, int comb){
        int rank = 0;
        int i = 0;
        //while(1 << i <= comb) i += 1;
        //printf("i = %d\n", i);
        int count = 0;
        for(; i < n; ++i){
            if(comb & (1 << i)){
                count += 1;
                rank += bin[i][count];
                //printf("i = %d, count = %d\n", i, count);
            }else{
                
            }
        }
        return rank;
    }

}