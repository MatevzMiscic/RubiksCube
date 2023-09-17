#pragma once

#include "../Settings.h"


struct table{
    // table of length len, where all each entry has two bits

    ll len;
    int* bits;
    
    table(ll n);
    ~table();

    void fill(int a);
    
    
    inline int get(uint index){
        //cout << index << "\n";
        return (bits[index >> 4] >> ((index & 15) << 1)) & 3;
    }

    inline void set(uint index, int val){
        int shift = (index & 15) << 1;
        bits[index >> 4] &= ~(3 << shift);
        bits[index >> 4] |= val << shift;
    }

    void print();
};
