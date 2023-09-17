#include <iostream>
#include <vector>
#include <deque>
#include "../preprocess/table.h"
#include "../moves/moves.h"
#include "../io/io.h"


using namespace std;


vector<ushort> cornermt;
vector<uint> layersmt(9676800);

table corner_arr(40320);
table layer_arr(967680);


template<typename type>
int length(vector<type>& movetable, table& arr, type u){
    int len = 0;
    bool run = true;
    while(run){
        run = false;
        int dist = (arr.get(u) + 2) % 3;
        for(int i = 0; i < 10; ++i){
            type v = movetable[10*u + i];
            if(arr.get(v) == dist){
                len += 1;
                u = v;
                run = true;
                break;
            }
        }
    }
    return len;
}



uint layer_coord(Cube& cube){
    std::array<byte, 4> slice;
    std::array<byte, 8> ud;
    for(int i = 0; i < 4; ++i) ud[i] = out(cube.edge[i]);
    for(int i = 0; i < 4; ++i) slice[i] = cube.edge[i + 4] - 4;
    for(int i = 0; i < 4; ++i) ud[i + 4] = out(cube.edge[i + 8]);
    return 24 * perm::rank(ud) + perm::rank(slice);
}


int h(Cube& cube){
    int len = length(cornermt, corner_arr, (ushort)cube.corner_coord());
    int len2 = length(layersmt, layer_arr, layer_coord(cube));
    printf("corner len: %d, layer len: %d\n", len, len2);
    if(len2 > len) len = len2;
    return len;
}


int main(){
    cmb::calcbin();
    cornermt = corner_movetable();
    io::read("../moves/layers.move", (int*)(&layersmt[0]), 4*layersmt.size());
    io::read("../preprocess/stage2_corner.bin", corner_arr.bits, (corner_arr.len + 3) / 4);
    io::read("../preprocess/stage2_layer.bin", layer_arr.bits, (layer_arr.len + 3) / 4);
    std::array<int, 10> index = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    vector<int> moves = {1, 5, 6, 3, 6, 2, 3, 9, 0, 0, 0, 1, 8, 7};
    Cube cube;
    h(cube);
    for(int i : moves){
        cube *= Cube::moves[index[i]];
        h(cube);
    }
    // there is an error in the corner pruning table
}