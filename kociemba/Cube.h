#pragma once

#include <array>
#include "Settings.h"

using namespace std;



class Cube{
    private:

    array<byte, 8> corner;
    array<byte, 8> twist;
    array<byte, 12> edge;
    array<byte, 12> flip;

    public:
    
    static Cube F, B, U, D, L, R;
    static Cube F2, B2, U2, D2, L2, R2;
    static Cube F3, B3, U3, D3, L3, R3;

    Cube();
    Cube(const array<byte, 8>& corner, const array<byte, 8>& twist, const array<byte, 12>& edge, const array<byte, 12>& flip);

    void operator*=(const Cube& cube);
    Cube operator*(const Cube& cube);

    int corner_coord();
    int twist_coord();
    int edge_coord();
    int flip_coord();

    void print();
};