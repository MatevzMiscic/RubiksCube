#pragma once

#include <array>
#include "../Settings.h"
#include "../Cube.h"



int twist_coord(std::array<byte, 8>& twist){
    int coord = 0;
    for(int i = 0; i < 7; ++i){
        coord = 3*coord + twist[i];
    }
    return coord;
}

std::array<short, 18*2187> twist_movetable(){
    std::array<short, 18*2187> movetable;
    std::array<byte, 8> twist = {};
    std::array<byte, 8> result = {};
    // go through all 2187 possible twist coordinates
    for(int i = 0; i < 2187; ++i){
        // go through all 18 moves
        for(int j = 0; j < 18; ++j){
            // calculate the product
            for(int k = 0; k < 8; ++k){
                result[k] = (Cube::moves[j].twist[k] + twist[Cube::moves[j].corner[k]]) % 3;
            }
            // find twist coordinates of the product
            movetable[18*i + j] = twist_coord(result);
        }
        // calculate twist for next coordinates
        for(int k = 6; k >= 0; --k){
            twist[7] += 2;
            if(twist[k] == 2){
                twist[k] = 0;
            }else{
                twist[k] += 1;
                break;
            }
        }
        twist[7] = twist[7] % 3;
    }
    return movetable;
}
