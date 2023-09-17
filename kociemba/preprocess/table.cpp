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

