#pragma once

#include <array>
#include <unordered_set>
#include <algorithm>
#include "../Settings.h"
#include "../Cube.h"
#include "../Symmetries.h"
#include "../math/Combination.h"
#include "../math/Permutation.h"





// returns coordinate of the twist
int twist_coord(std::array<byte, 8>& twist){
    int coord = 0;
    for(int i = 0; i < 7; ++i){
        coord = 3*coord + twist[i];
    }
    return coord;
}

// returns move table for twist coordinate
std::vector<ushort> twist_movetable(){
    std::vector<ushort> movetable(18*2187);
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
            movetable[18*i + j] = short(twist_coord(result));
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





// returns move table for flip coordinate
std::vector<ushort> flip_movetable(){
    std::vector<ushort> movetable(18*2048);
    // go through all 2048 flip coordinates
    for(int i = 0; i < 2048; ++i){
        //printf("par: %d\n", __builtin_parity(i));
        int flip = i | (__builtin_parity(i) << 11);
        // go through all 18 moves
        for(int j = 0; j < 18; ++j){
            // calculate the product
            int result = 0;
            for(int k = 0; k < 12; ++k){
                result |= ((Cube::moves[j].flip[k] ^ (flip >> Cube::moves[j].edge[k])) & 1) << k;
            }
            //save to move table
            movetable[18*i + j] = ushort(result & 2047);
        }
    }
    return movetable;
}





// returns move table for slice coordinate
std::vector<ushort> slice_movetable(){
    std::vector<ushort> movetable(18*495);
    // go through all 495 slice coordinates
    int slice_coord = 0;
    for(int slice = 0; slice < 4096; ++slice){
        if(__builtin_popcount(slice) != 4){
            continue;
        }
        // go through all 18 moves
        for(int j = 0; j < 18; ++j){
            // calculate the product
            int result = 0;
            for(int k = 0; k < 12; ++k){
                if(slice & (1 << Cube::moves[j].edge[k])){
                    result |= 1 << k;
                }
            }
            movetable[18*slice_coord + j] = ushort(cmb::rank(12, 4, result));
        }
        slice_coord += 1;
    }
    return movetable;
}







// returns move table for corner coordinate
std::vector<ushort> corner_movetable(){
    std::array<int, 10> index = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    std::vector<ushort> movetable(18*40320);
    std::array<byte, 8> perm = {0, 1, 2, 3, 4, 5, 6, 7};
    std::array<byte, 8> result;
    int coord = 0;
    do{
        for(int i = 0; i < 10; ++i){
            for(int j = 0; j < 8; ++j){
                result[j] = perm[Cube::moves[index[i]].corner[j]];
            }
            movetable[18*coord + i] = ushort(perm::rank(result));
        }
        coord += 1;
    }while(next_permutation(perm.begin(), perm.end()));
    return movetable;
}










inline int out(int a){
    if(a < 4) return a;
    return a - 4;
}

// returns move table for corner coordinate (size = 10*967680)
std::vector<uint> layers_movetable(){
    std::array<int, 10> index = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    std::vector<uint> movetable(10*967680);
    std::array<byte, 4> slice = {4, 5, 6, 7};
    std::array<byte, 8> ud = {0, 1, 2, 3, 8, 9, 10, 11};
    std::array<byte, 12> edge;
    std::array<byte, 12> result;
    std::array<byte, 4> slice_result;
    std::array<byte, 8> ud_result;
    int ud_coord = 0;
    int slice_coord = 0; // not the real slice coord
    do{
        slice_coord = 0;
        do{
            for(int i = 0; i < 4; ++i) edge[i] = ud[i];
            for(int i = 0; i < 4; ++i) edge[i + 4] = slice[i];
            for(int i = 0; i < 4; ++i) edge[i + 8] = ud[i + 4];
            //for(byte a : edge) printf("%d, ", a);
            //printf("\n");
            for(int j = 0; j < 10; ++j){
                for(int i = 0; i < 12; ++i){
                    result[i] = edge[Cube::moves[index[j]].edge[i]];
                }
                for(int i = 0; i < 4; ++i) ud_result[i] = out(result[i]);
                for(int i = 0; i < 4; ++i) slice_result[i] = result[i + 4] - 4;
                for(int i = 0; i < 4; ++i) ud_result[i + 4] = out(result[i + 8]);
                movetable[10*(24*ud_coord + slice_coord) + j] = 24*perm::rank(ud_result) + perm::rank(slice_result);
            }
            slice_coord += 1;
        }while(next_permutation(slice.begin(), slice.end()));
        ud_coord += 1;
        if(ud_coord % 403 == 0) printf("%d / 100\n", ud_coord / 403);
    }while(next_permutation(ud.begin(), ud.end()));
    return movetable;
}












class EdgeFlip{
    public:

    std::array<byte, 12> edge;
    int flip;

    EdgeFlip(){
        edge = {};
        flip = 0;
    }

    EdgeFlip(std::array<byte, 12>& edge, int flip){
        this->edge = edge;
        this->flip = flip;
    }

    EdgeFlip(Cube& cube){
        edge = cube.edge;
        flip = 0;
        for(int i = 0; i < 12; ++i){
            if(cube.flip[i] == 1){
                flip |= 1 << i;
            }
        }
    }

    // multiplies this EdgeFlip with other
    void operator*=(const EdgeFlip& other){
        std::array<byte, 12> edge;
        int flip = 0;
        for(int i = 0; i < 12; ++i){
            flip |= (((other.flip >> i) ^ (this->flip >> other.edge[i])) & 1) << i;
            edge[i] = this->edge[other.edge[i]];
        }
        this->edge = edge;
        this->flip = flip;
    }

    // returns the product of this EdgeFlip with other
    EdgeFlip operator*(const EdgeFlip& other){
        EdgeFlip prod;
        for(int i = 0; i < 12; ++i){
            prod.flip |= (((other.flip >> i) ^ (this->flip >> other.edge[i])) & 1) << i;
            prod.edge[i] = this->edge[other.edge[i]];
        }
        return prod;
    }

    // retruns the inverse of this EdgeFlip
    EdgeFlip inv(){
        EdgeFlip inverse;
        for(int i = 0; i < 12; ++i){
            inverse.flip |= ((this->flip >> i) & 1) << this->edge[i];
            inverse.edge[this->edge[i]] = i;
        }
        return inverse;
    }

    
    // returns the coordinate that uniquly defines this EdgeFlip
    inline ll coord(){
        return (ll(edge_coord()) << 11) | flip_coord();
    }

    // returns edge coordinate
    inline int edge_coord(){
        return perm::rank(edge);
    }

    // returns flip coordinate in 'is replaced by' representation
    inline int flip_coord(){
        return flip & 2047;
    }

    // returns slice coordinate in 'is replaced by' representation
    inline int slice_coord(){
        int coord = 0;
        for(int i = 0; i < 11; ++i){
            if(4 <= edge[i] && edge[i] <= 7){
                coord |= 1 << i;
            }
        }
        return cmb::rank(12, 4, coord);
    }

    // returns flipslice coordinate in 'is replaced by' representation
    inline int flipslice_coord(){
        return (slice_coord() << 11) | flip_coord();
    }

    void print(){
        std::printf("(");
        for(byte c : edge) printf("%d ", c);
        std::printf("), (");
        for(int i = 0; i < 12; ++i) printf("%d ", ((1 << i) & flip) >> i);
        std::printf(")\n");
    }
};



void printbin(int n){
    for(int i = 0; i < 12; ++i){
        if(n & (1 << i)){
            printf("1 ");
        }else{
            printf("0 ");
        }
    }
    printf("\n");
}




// returns the number of one bits of num among the least significant n bits
int countbits(int num, int n){
    int count = 0;
    for(int i = 0; i < n; ++i){
        if(num & (1 << i)) count += 1;
    }
    return count;
}

int representative(EdgeFlip& a){
    std::array<EdgeFlip, 16> symmetries;
    //sym::compute();
    for(int i = 0; i < 16; ++i){
        symmetries[i] = EdgeFlip(sym::symmetries[i]);
    }
    //ll repr = a.coord();
    int repr = 1e9;
    for(EdgeFlip& s : symmetries){
        //(s.inv() * a * s).print();
        //ll coord = (s.inv() * a * s).coord();
        int coord = (s.inv() * a * s).flipslice_coord();
        if(coord < repr){
            repr = coord;
        }
    }
    return repr;
}

std::array<int, 100000> represent_table(){
    int count = 0; // counts the number of equivalence classes
    std::array<int, 100000> represents = {}; // flipslice coordinates of representatives
    std::unordered_set<ll> set; // stores coordinates of elements of classes whose representatives were already found
    // compute symmetries
    std::array<EdgeFlip, 16> symmetries;
    sym::compute();
    for(int i = 0; i < 16; ++i){
        symmetries[i] = EdgeFlip(sym::symmetries[i]);
    }
    // go through all slices
    int slice_coord = 0;
    for(int slice = 0; slice < 2048; ++slice){

        if(slice % 20 == 0) std::printf("%d / 100\n", slice / 20);

        if(countbits(slice, 12) != 4){
            continue;
        }
        // construct a representative edge of a slice
        std::array<byte, 12> edge;
        int slice_edge = 4;
        int non_slice_edge = 0;
        for(int i = 0; i < 12; ++i){
            if(slice & (1 << i)){
                edge[i] = slice_edge;
                slice_edge += 1;
            }else{
                edge[i] = non_slice_edge;
                non_slice_edge += 1;
                if(non_slice_edge == 4) non_slice_edge = 8;
            }
        }
        // go through all flips
        for(int flip_coord = 0; flip_coord < 2048; ++flip_coord){
            int parity = __builtin_parity(flip_coord);
            int flip = flip_coord | (parity << 11);
            int flipslice = (slice_coord << 11) | flip_coord;
            // if this equivalence class was already found continue
            EdgeFlip repr(edge, flip);
            if(set.find(repr.coord()) != set.end()){
                continue;
            }
            // else find representative of a class
            for(EdgeFlip& s : symmetries){
                EdgeFlip sym = s.inv() * repr * s;
                int flipslice_sym = sym.flipslice_coord();
                set.insert(sym.coord());
                if(flipslice_sym < flipslice){
                    flipslice = flipslice_sym;
                }
            }
            represents[count] = flipslice;
            count += 1;
        }
        slice_coord += 1;
    }
    std::printf("count = %d\n", count);
    return represents;
}
