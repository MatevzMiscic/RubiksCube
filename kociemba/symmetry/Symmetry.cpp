#include <iostream>
#include <bitset>
#include <vector>
#include <deque>
#include <algorithm>
#include "symmetry.h"
#include "../Settings.h"
#include "../math/Permutation.h"
#include "../moves/moves.h"


using namespace std;





// third turn around an axis through FUR and BDL corners
Cube sym::thirdturn(array<byte, 8>{2, 6, 7, 3, 1, 5, 4, 0}, array<byte, 8>{1, 2, 1, 2, 2, 1, 2, 1}, array<byte, 12>{6, 10, 7, 2, 1, 9, 11, 3, 5, 8, 4, 0}, array<byte, 12>{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});

// half turn around an axis that goes through the centers of F and B faces
Cube sym::halfturn(array<byte, 8>{5, 4, 7, 6, 1, 0, 3, 2}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{8, 11, 10, 9, 5, 4, 7, 6, 0, 3, 2, 1}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

// quorter turn around an axis that goes through the centers of D and U faces
Cube sym::quarturn(array<byte, 8>{3, 0, 1, 2, 7, 4, 5, 6}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10}, array<byte, 12>{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0});

// reflection over the plane that goes throug the middle of the slice between L and R faces
Cube sym::reflect(array<byte, 8>{1, 0, 3, 2, 5, 4, 7, 6}, array<byte, 8>{3, 3, 3, 3, 3, 3, 3, 3}, array<byte, 12>{0, 3, 2, 1, 5, 4, 7, 6, 8, 11, 10, 9}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

array<Cube, 16> sym::symmetries;
array<Cube, 16> sym::symmetries_inv;


// fills the symmetries array
void sym::compute(){
    Cube a, b, c;
    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 4; ++j){
            for(int k = 0; k < 2; ++k){
                symmetries[8*i + 2*j + k] = a * b * c;
                c *= reflect;
            }
            b *= quarturn;
        }
        a *= halfturn;
    }
    for(int i = 0; i < 16; ++i){
        symmetries_inv[i] = symmetries[i].inv();
    }
}





// returns the symmetry table for corner coordinate
vector<ushort> sym::corner_st(){
    compute();
    vector<ushort> st(40320*16);
    array<byte, 8> corner = {0, 1, 2, 3, 4, 5, 6, 7};
    int coord = 0;
    do{
        for(int i = 0; i < 16; ++i){
            ushort result = perm::rank(perm::mul(perm::mul(sym::symmetries[i].corner, corner), sym::symmetries_inv[i].corner));
            st[(coord << 4) | i] = result;
        }
        coord += 1;
    }while(next_permutation(corner.begin(), corner.end()));
    return st;
}




// parameter st must be symmetry table for corner coordinate
// returns vector of representatives for symmetry classes of corner coordinate
vector<ushort> sym::corner_rt(vector<ushort>& st){
    vector<ushort> represent;
    bitset<40320> set;
    for(int coord = 0; coord < 40320; ++coord){
        ushort min = coord;
        if(set[coord] == false){
            for(int i = 0; i < 16; ++i){
                ushort result = st[(coord << 4) | i];
                if(result < min) min = result;
                set[result] = true;
            }
            represent.push_back(min);
        }
    }
    return represent;
}

// initializes vectors to convert between raw and symmetric corner coordinates
void sym::corner_conversion(vector<ushort>& sym_to_raw, vector<ushort>& raw_to_sym){
    vector<ushort> st = corner_st(); // compute symmetry table
    vector<ushort> rt = corner_rt(st); // compute representative table
    sym_to_raw.assign(16*2768, 0);
    raw_to_sym.assign(40320, 0);
    for(int equiv = 0; equiv < 2768; ++equiv){
        for(int sym = 0; sym < 16; ++sym){
            ushort sym_coord = (equiv << 4) | sym;
            ushort raw_coord = st[(rt[equiv] << 4) | sym];
            sym_to_raw[sym_coord] = raw_coord;
            raw_to_sym[raw_coord] = sym_coord;
        }
    }
}









inline int out(int a){
    if(a < 4) return a;
    return a - 4;
}

// returns the inverse symmetry table for layer coordinate
vector<uint> sym::layer_ist(){
    compute();
    vector<uint> ist(967680*16);
    std::array<byte, 4> slice = {4, 5, 6, 7};
    std::array<byte, 8> ud = {0, 1, 2, 3, 8, 9, 10, 11};
    array<byte, 12> edge;
    array<byte, 12> result;
    std::array<byte, 4> slice_result;
    std::array<byte, 8> ud_result;
    int coord = 0;
    //int ud_coord = 0;
    do{
        for(int i = 0; i < 4; ++i) edge[i] = ud[i];
        for(int i = 8; i < 12; ++i) edge[i] = ud[i - 4];
        do{
            for(int i = 0; i < 4; ++i) edge[i + 4] = slice[i];
            int sign = perm::sign(edge);
            for(int i = 0; i < 16; ++i){
                result = perm::mul(perm::mul(sym::symmetries_inv[i].edge, edge), sym::symmetries[i].edge);
                for(int i = 0; i < 4; ++i) ud_result[i] = out(result[i]);
                for(int i = 0; i < 4; ++i) slice_result[i] = result[i + 4] - 4;
                for(int i = 8; i < 12; ++i) ud_result[i - 4] = out(result[i]);
                ist[(coord << 4) | i] = 24 * perm::rank(ud_result) + perm::rank(slice_result);
            }
            coord += 1;
        }while(std::next_permutation(slice.begin(), slice.end()));
        //if(ud_coord % 4032 == 0) printf("%d / 10\n", ud_coord / 4032);
        //ud_coord++;
    }while(std::next_permutation(ud.begin(), ud.end()));
    return ist;
}

void sym::layer_compressed_ist(vector<uint>& even_ist, vector<uint>& odd_ist){
    even_ist.assign(16*483840, 0);
    odd_ist.assign(16*483840, 0);
    vector<uint> even;
    vector<uint> odd;
    vector<uint> layer;
    move::layer_conversion(even, odd, layer);
    vector<uint> ist = layer_ist();
    for(int i = 0; i < 483840; ++i){
        for(int j = 0; j < 16; ++j){
            even_ist[16*i + j] = layer[ist[16*even[i] + j]];
            odd_ist[16*i + j] = layer[ist[16*odd[i] + j]];
        }
    }
}