#include <iostream>
#include <bitset>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include "Symmetries.h"
#include "../math/Permutation.h"
#include "../preprocess/table.h"
#include "../io/io.h"
#include "../Settings.h"


using namespace std;


struct sym_coord{
    ushort equiv;
    ubyte symmetry;
};



unordered_map<ushort, ushort> equiv;
vector<ushort> repr;



vector<ushort> corner_rt(){
    sym::compute();
    vector<ushort> represent;
    bitset<40320> set;
    array<byte, 8> corner = {0, 1, 2, 3, 4, 5, 6, 7};
    int coord = 0;
    do{
        if(set[coord] == false){
            ushort min = coord;
            for(int i = 0; i < 16; ++i){
                ushort result = perm::rank(perm::mul(perm::mul(sym::symmetries[i].corner, corner), sym::symmetries_inv[i].corner));
                if(result < min) min = result;
                set[result] = true;
            }
            represent.push_back(min);
        }
        coord += 1;
    }while(next_permutation(corner.begin(), corner.end()));
    return represent;
}



int main(){
    vector<ushort> represent = corner_rt();
    printf("size = %d\n", represent.size());
    /*
    sort(represent.begin(), represent.end());
    auto it = unique(represent.begin(), represent.end());
    represent.resize(distance(represent.begin(), it));
    sort(represent.begin(), represent.end());
    printf("size = %d\n", represent.size());
    */
    /*
    for(int j = 0; j < 18; ++j){
        ushort min = 60000;
        for(int i = 0; i < 16; ++i){
            ushort result = perm::rank(perm::mul(perm::mul(sym::symmetries[i].corner, Cube::moves[j].corner), sym::symmetries_inv[i].corner));
            if(result < min) min = result;
        }
        printf("%d\n", min);
    }
    */
}