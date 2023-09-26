#pragma once

#include <array>
#include <vector>
#include "Settings.h"




class Cube{
    private:

    public:
    
    std::array<byte, 8> corner;
    std::array<byte, 8> twist;
    std::array<byte, 12> edge;
    std::array<byte, 12> flip;
    
    
    static Cube F, B, U, D, L, R;
    static Cube F2, B2, U2, D2, L2, R2;
    static Cube F3, B3, U3, D3, L3, R3;
    
    static std::array<Cube, 18> moves;

    Cube();
    Cube(const std::array<byte, 8>& corner, const std::array<byte, 8>& twist, const std::array<byte, 12>& edge, const std::array<byte, 12>& flip);

    void shuffle();
    std::vector<int> shuffle(std::vector<int>& index, int moves);

    bool in_subgroup();
    bool is_solved();
    bool operator==(const Cube& other) const;

    void operator*=(const Cube& cube);
    Cube operator*(const Cube& cube);

    Cube inv();

    uint corner_coord();
    uint twist_coord();
    uint edge_coord();
    uint flip_coord();
    uint slice_coord();
    uint flipslice_coord();

    void print();
};