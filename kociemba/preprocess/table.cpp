#include <iostream>
#include "table.h"


using namespace std;

table::table(ll n){
    len = n;
    bits = new int[n/16 + 1];
}

table::~table(){
    delete[] bits;
}

void table::fill(int a){
    int flag = 0;
    for(int i = 0; i < 16; ++i){
        flag = (flag << 2) | a;
    }
    //printb(a);
    //printb(flag);
    for(int i = 0; i < len/16 + 1; ++i){
        bits[i] = flag;
    }
}

void table::print(){
    for(int i = 0; i < len; ++i){
        printf("%d ", get(i));
    }
    printf("\n");
}

void table::stats(){
    uint freq[4] = {0, 0, 0, 0};
    for(int i = 0; i < len; ++i){
        freq[get(i)] += 1;
    }
    printf("0: %u, 1: %u, 2: %u, 3: %u\n", freq[0], freq[1], freq[2], freq[3]);
}
