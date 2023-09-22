#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include <algorithm>
#include "../preprocess/table.h"
#include "../moves/moves.h"
#include "../io/io.h"


using namespace std;


vector<ushort> cornermt;
vector<uint> layermt(9676800);

table corner_arr(40320);
table layer_arr(967680);

int cornercount = 0;
int layercount = 0;
int both = 0;





inline int out(int a){
    if(a < 4) return a;
    return a - 4;
}
uint layer_coord(Cube& cube){
    std::array<byte, 4> slice;
    std::array<byte, 8> ud;
    for(int i = 0; i < 4; ++i) ud[i] = out(cube.edge[i]);
    for(int i = 0; i < 4; ++i) slice[i] = cube.edge[i + 4] - 4;
    for(int i = 0; i < 4; ++i) ud[i + 4] = out(cube.edge[i + 8]);
    return 24 * perm::rank(ud) + perm::rank(slice);
}



struct coord{
    uint layer;
    ushort corner;

    coord(uint layer, uint corner) : layer(layer), corner(corner){}
    coord(Cube& cube){
        layer = layer_coord(cube);
        corner = cube.corner_coord();
    }

    inline coord neigh(int i){
        return coord(layermt[10*layer + i], cornermt[10*corner + i]);
    }

    bool operator==(const coord& other){
        return layer == other.layer && corner == other.corner;
    }

    inline bool is_id(){
        return layer == 0 && corner == 0;
    }

    inline void print(){
        printf("corner: %d, layer: %d\n", corner, layer);
    }
};







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


int h(Cube& cube){
    int len = length(cornermt, corner_arr, (ushort)cube.corner_coord());
    int len2 = length(layermt, layer_arr, layer_coord(cube));
    printf("corner len: %d, layer len: %d\n", len, len2);
    if(len2 > len) len = len2;
    return len;
}


inline int h2(uint layer_coord){
    return length(layermt, layer_arr, layer_coord);
}

inline int h2(coord& cube){
    int len1 = length(layermt, layer_arr, cube.layer);
    int len2 = length(cornermt, corner_arr, cube.corner);
    if(len2 < len1){
        layercount += 1;
        return len1;
    }else if(len2 == len1){
        both += 1;
        return len1;
    }
    cornercount += 1;
    return len2;
}


bool search(vector<coord>& path, int g, int depth){
    coord u = path.back();
    if(g + h2(u) > depth) return false;
    if(u.is_id()) return true;
    for(int i = 0; i < 10; ++i){
        coord v = u.neigh(i);
        if(find(path.begin(), path.end(), v) == path.end()){
            path.push_back(v);
            bool found = search(path, g + 1, depth);
            if(found) return true;
            path.pop_back();
        }
    }
    return false;
}

vector<coord> ida(coord u){
    int depth = h2(u);
    vector<coord> path = {u};
    for(int d = depth; d <= 18; ++d){
        printf("depth = %d\n", d);
        if(search(path, 0, d)){
            return path;
        }
    }
    printf("Not found.\n");
    return path;
}




int main(){
    cmb::calcbin();
    cornermt = move::corner_movetable();
    io::read("../moves/layers.move", (int*)(&layermt[0]), 4*layermt.size());
    io::read("../preprocess/stage2_corner.bin", corner_arr.bits, (corner_arr.len + 3) / 4);
    io::read("../preprocess/stage2_layer.bin", layer_arr.bits, (layer_arr.len + 3) / 4);
    vector<int> index = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    Cube cube;
    /*
    vector<int> moves = {1, 5, 6, 3, 6, 2, 3, 9, 0, 0, 0, 1, 8, 7};
    h(cube);
    for(int i : moves){
        cube *= Cube::moves[index[i]];
        h(cube);
    }
    */
    // there is an error in the corner pruning table
    
    for(int i = 0; i < 10; ++i){
        cube.shuffle(index, 50);
        vector<coord> path = ida(coord(cube));
        printf("len: %d\n", path.size());
    }
    //for(coord u : path) u.print();
    
    printf("corner count: %d, layer count: %d, both: %d\n", cornercount, layercount, both);
}