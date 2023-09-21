#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include "../Settings.h"
#include "../moves/moves.h"
#include "../io/io.h"
#include "table.h"

using namespace std;



vector<ushort> st;
vector<ushort> rt;
vector<ushort> sym_to_raw;
vector<ushort> raw_to_sym;


vector<ushort> cornermt;
vector<uint> layermt[2];
vector<uint> ist[2];

vector<int> indeces = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
vector<int> inv = {0, 1, 6, 3, 4, 5, 2, 7, 8, 9, 10, 15, 12, 13, 14, 11};
array<uint, 10> move_sign;



struct coord{
    ushort corner;
    uint layer;

    coord() : corner(0), layer(0){}
    coord(ushort corner, uint layer) : corner(corner), layer(layer){}

    inline int index(){
        return 483840 * corner + (layer >> 1);
    }

    inline coord neigh(int i){
        uint sign = layer & 1;
        uint new_sign = move_sign[i] ^ sign;
        ushort corner_sym = cornermt[10*(corner << 4) + i];
        //printf("sign: %d, corner class: %d, corner sym: %d, layer: %d, layer turned: %d\n", sign, corner_sym >> 4, corner_sym & 15, layermt[sign][10 * (layer >> 1) + i], ist[sign][(layermt[sign][10 * (layer >> 1) + i] << 4) | (corner_sym & 15)]);
        return coord(corner_sym >> 4, (ist[new_sign][(layermt[sign][10 * (layer >> 1) + i] << 4) | (corner_sym & 15)] << 1) | new_sign);
    }

    inline void print() const {
        printf("corner: %d, layer %d, sign: %d\n", corner, layer >> 1, layer & 1);
    }
};







inline int out(int a){
    if(a < 4) return a;
    return a - 4;
}

int layers_compressed_coord(Cube& cube){
    std::array<byte, 4> slice;
    std::array<byte, 8> ud;
    for(int i = 0; i < 4; ++i) ud[i] = out(cube.edge[i]);
    for(int i = 0; i < 4; ++i) slice[i] = cube.edge[i + 4] - 4;
    for(int i = 8; i < 12; ++i) ud[i - 4] = out(cube.edge[i]);
    return 12 * perm::rank(ud) + perm::rank_sign(slice, perm::sign(slice));
}

coord get_one_coord(Cube& cube){
    ushort corner = raw_to_sym[cube.corner_coord()] >> 4;
    ushort repr = rt[corner];
    int sign = perm::sign(cube.corner);
    for(int i = 0; i < 16; ++i){
        Cube conj = sym::symmetries[i] * cube * sym::symmetries[i].inv();
        ushort result = perm::rank(conj.corner);
        if(result == repr){
            uint layer = layers_compressed_coord(conj);
            return coord(corner, (layer << 1) | sign);
        }else if(result < repr) printf("not minimum\n");
    }
    printf("min not acheaved\n");
    coord out;
    return out;
}

vector<coord> get_coords(coord& v){
    vector<coord> out;
    ushort repr = rt[v.corner];
    int sign = v.layer & 1;
    for(int i = 0; i < 16; ++i){
        ushort result = sym_to_raw[(v.corner << 4) | i];
        if(result == repr){
            uint layer = ist[sign][((v.layer >> 1) << 4) | inv[i]];
            out.push_back(coord(v.corner, (layer << 1) | sign));
        }else if(result < repr) printf("not minimum\n");
    }
    if(out.size() == 0) printf("min not acheaved\n");
    return out;
}









void pruning_table(table& arr, int bound){
    int distance = 0;
    coord s;
    deque<coord> queue = {s};
    arr.fill(3);
    arr.set(s.index(), 0);

    int count = 0;

    while(!queue.empty()){
        count += 1;
        coord u = queue.front();
        queue.pop_front();

        //printf("  node %d found\n", u.index());

        int dist = arr.get(u.index()) + 1;
        if(dist == 3) dist = 0;
        if(distance % 3 != dist){
            printf("Up to dist %d, count: %d\n", distance - 1, count - 1);
            count = 1;
            distance += 1;
        }

        //printf("u = %d, dist = %d\n", index_u, distance-1);
        for(int i = 0; i < 10; ++i){
            coord v = u.neigh(i);
            uint index = v.index();
            if(arr.get(index) == 3){
                //arr.set(index, dist);
                for(coord& w : get_coords(v)) arr.set(w.index(), dist);
                if(distance < bound) queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}




vector<int> solve(Cube& cube, table& arr){
    vector<int> moves;
    coord c = get_one_coord(cube);
    c.print();
    bool run = true;
    while(run){
        run = false;
        int dist = (arr.get(c.index()) + 2) % 3;
        printf("c = %d, dist = %d, \n", c.index(), arr.get(c.index()));
        for(int i = 0; i < 10; ++i){
            coord d = c.neigh(i);
            if(arr.get(d.index()) == dist){
                printf("move %d\n", indeces[i]);
                moves.push_back(i);
                run = true;
                c = d;
                break;
            }
        }
    }
    return moves;
}


int main(){

    vector<int> index = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};

    cornermt = move::corner_sym_movetable();
    move::layers_compressed_movetable(layermt[0], layermt[1]);
    ist[0].assign(16*483840, 0);
    ist[1].assign(16*483840, 0);
    bool read = true;
    read &= io::read("../symmetry/layer_even.sym", (int*)(&ist[0][0]), 4*ist[0].size());
    read &= io::read("../symmetry/layer_odd.sym", (int*)(&ist[1][0]), 4*ist[1].size());
    printf("tables read: %d\n", read);

    st = sym::corner_st();
    rt = sym::corner_rt(st);
    sym::corner_conversion(sym_to_raw, raw_to_sym);
    sym::compute();

    for(int i = 0; i < 10; ++i) move_sign[i] = perm::sign(Cube::moves[index[i]].corner);

    table arr(2768 * 483840);
    pruning_table(arr, 4);

    // testing
    /*
    Cube c;
    vector<int> moves = {0, 7, 2};
    for(int i : moves){
        printf("%d, ", indeces[i]);
        c *= Cube::moves[indeces[i]];
    }
    printf("\n");
    solve(c, arr);
    */
    
    for(int i = 0; i < 5000; ++i){
        Cube cube;
        cube.shuffle(indeces, 4);
        coord c = get_one_coord(cube);
        int dist = arr.get(c.index());
        if(dist == 3) cube.print();
    }
}