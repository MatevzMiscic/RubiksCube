#include <iostream>
#include "Set.h"

/*
void printbin(ulong n){
    for(int i = 63; i >= 0; --i){
        if((1ll << i) & n) printf("1");
        else printf("0");
        if(i % 5 == 0) printf(" ");
    }
    printf("\n");
}
//*/

Set::Set(int n){
    b = 0; // b will count bits needed to represent n-1
    int flag = n - 1;
    while(flag > 0){
        flag >>= 1;
        b += 1;
    }
    bits = 0; // data
    mul = 0; // multiplier to set all fields to some value
    for(int i = n - 1; i >= 0; --i){
        bits = (bits << (b + 1)) | ulong(i);
        mul = (mul << (b + 1)) | ulong(1);
    }
    sep = mul << b; // separator bits
    mod = (2ll << b) - 1; // modulo for summing all fields
}

int Set::rank(int x){
    ulong c = (mul * x) | sep; // set every field to x and set separator bits
    ulong num = (c - bits) & sep; // separator bits of elements less than or equal to x are set to one
    return ((num >> b) % mod) - 1; // counts those bits and subtracts one
}

void Set::del(int x){
    int r = rank(x);
    ulong left = (bits >> ((r + 1) * (b + 1))) << (r * (b + 1));
    ulong right = bits & ((1ll << (r * (b + 1))) - 1);
    bits = left | right; // removing field that contains x
    mul >>= b + 1; // there is one field less
    sep >>= b + 1; // there is one field less
}

void Set::print(){
    //printbin(bits);
    ulong count = mul;
    ulong copy = bits;
    while(count > 0){
        count >>= b + 1;
        printf("%d ", copy & ((1ll << b) - 1));
        copy >>= b + 1;
    }
    printf("\n");
}