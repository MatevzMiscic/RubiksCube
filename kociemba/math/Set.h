#pragma once

#include "../Settings.h"

class Set{
    private:

    ulong bits; // data
    ulong mul; // multiplier to set all fields to some value
    ulong sep; // separator bits between fields
    int b; // number of bits for each field
    int mod; // modulo for summing all fields

    public:
    // creates new Set with integers 0,...,n-1
    // n has to be less that or equal to 12
    Set(int n);

    // returns number of integers in the set that are less than x
    int rank(int x);

    // deletes integer x from set
    void del(int x);

    // prints set
    void print();
};