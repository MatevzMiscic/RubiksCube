#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include <bitset>
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

bitset<2768> symmetric_pos;
bitset<2768> corner_sign;


struct coord{
    ushort corner;
    uint layer;

    coord() : corner(0), layer(0){}
    coord(ushort corner, uint layer) : corner(corner), layer(layer){}

    inline uint index(){
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

vector<coord> get_coords(const coord& v){
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


bool diff(const vector<coord>& vec){
    for(int i = 1; i < vec.size(); ++i){
        if(vec[i].corner != vec[0].corner || vec[i].layer != vec[0].layer) return true;
    }
    return false;
}
void printv(const vector<coord>& v){
    for(const coord& c : v) c.print();
    printf("\n");
}




bitset<2768> symmetric(){
    bitset<2768> out;
    for(int i = 0; i < 2768; ++i){
        uint coord = rt[i];
        for(int j = 1; j < 16; ++j){
            if(st[(coord << 4) | j] == coord){
                out[i] = true;
                break;
            }
        }
    }
    return out;
}
bitset<2768> cornersign(){
    bitset<2768> out;
    bitset<40320> sign;
    array<byte, 8> perm = {0, 1, 2, 3, 4, 5, 6, 7};
    int coord = 0;
    do{
        sign[coord] = perm::sign(perm);
        coord += 1;
    }while(next_permutation(perm.begin(), perm.end()));
    for(int i = 0; i < 2768; ++i){
        out[i] = sign[rt[i]];
    }
    return out;
}




void pruning_table(table& arr, int bound){
    int distance = 0;
    coord s;
    deque<coord> queue = {s};
    arr.fill(3);
    arr.set(s.index(), 0);

    int depth_count = 0;
    int count = 0;

    while(!queue.empty()){
        count += 1;
        depth_count += 1;
        coord u = queue.front();
        queue.pop_front();

        //printf("  node %d found\n", u.index());

        int dist = arr.get(u.index()) + 1;
        if(dist == 3) dist = 0;
        if(distance % 3 != dist){
            printf("Up to dist %d, count: %d\n", distance - 1, depth_count - 1);
            depth_count = 1;
            distance += 1;
        }

        //printf("u = %d, dist = %d\n", index_u, distance-1);
        for(int i = 0; i < 10; ++i){
            coord v = u.neigh(i);
            uint index = v.index();
            if(arr.get(index) == 3){
                //arr.set(index, dist);
                //for(coord& w : get_coords(v)) arr.set(w.index(), dist);
                if(symmetric_pos[v.corner]){
                    for(coord& w : get_coords(v)) arr.set(w.index(), dist);
                }else{
                    arr.set(index, dist);
                    /*
                    vector<coord> vec = get_coords(v);
                    if(diff(vec)) printv(vec);
                    */
                }
                if(distance < bound) queue.push_back(v);
            }
        }
    }
    printf("Up to dist %d, count: %d\n", distance - 1, depth_count);
    printf("count = %d\n", count);
}

void forward_search(table& arr, int start, int end){
    for(int d = start; d <= end; ++d){
        int prev = (d + 2) % 3;
        int dist = d % 3;
        printf("prev = %d, dist = %d\n", prev, dist);
        uint index = 0;
        for(uint corner = 0; corner < 2768; ++corner){
            uint sign = corner_sign[corner];
            if(corner % 28 == 0) printf("%d / 100\n", corner / 28);
            for(uint layer = 0; layer < 483840; ++layer){
                if(arr.get(index) == prev){
                    for(int i = 0; i < 10; ++i){
                        ushort corner_sym = cornermt[10*(corner << 4) + i];
                        //coord v(corner_sym >> 4, ist[move_sign[i] ^ sign][(layermt[sign][10 * layer + i] << 4) | (corner_sym & 15)]);
                        coord v(corner_sym >> 4, (ist[move_sign[i] ^ sign][(layermt[sign][10 * layer + i] << 4) | (corner_sym & 15)] << 1) | (move_sign[i] ^ sign));
                        uint result = v.index();
                        if(arr.get(result) == 3){
                            if(symmetric_pos[v.corner]){
                                for(coord& w : get_coords(v)) arr.set(w.index(), dist);
                            }else{
                                arr.set(result, dist);
                            }
                        }
                    }
                }
                index += 1;
            }
        }
    }
}


void backward_search(table& arr, int start, int end){
    for(int d = start; d <= end; ++d){
        int prev = (d + 2) % 3;
        int dist = d % 3;
        printf("prev = %d, dist = %d\n", prev, dist);
        uint index = 0;
        for(uint corner = 0; corner < 2768; ++corner){
            uint sign = corner_sign[corner];
            if(corner % 28 == 0) printf("%d / 100\n", corner / 28);
            for(uint layer = 0; layer < 483840; ++layer){
                if(arr.get(index) == 3){
                    for(int i = 0; i < 10; ++i){
                        ushort corner_sym = cornermt[10*(corner << 4) + i];
                        uint result = 483840 * (corner_sym >> 4) + ist[move_sign[i] ^ sign][(layermt[sign][10 * layer + i] << 4) | (corner_sym & 15)];
                        if(arr.get(result) == prev){
                            arr.set(index, dist);
                        }
                    }
                }
                index += 1;
            }
        }
    }
}





struct coord2{
    uint corner; // symmetric corner coordinate
    uint layer; // compressed layer coordinate
    uint sign; // sign of corner

    coord2(uint corner, uint layer, uint sign) : corner(corner), layer(layer), sign(sign){}

    coord2(Cube& cube){
        corner = raw_to_sym[cube.corner_coord()];
        layer = layers_compressed_coord(cube);
        sign = perm::sign(cube.corner);
    }

    inline uint index(){
        return 483840 * (corner >> 4) + ist[sign][(layer << 4) | (corner & 15)];
    }

    inline coord2 neigh(int i){
        return coord2(cornermt[10 * corner + i], layermt[sign][10 * layer + i], move_sign[i] ^ sign);
    }

    inline void print() const {
        printf("corner: %d, layer %d, sign: %d\n", corner >> 4, layer, sign);
    }

};



vector<int> solve(Cube& cube, table& arr){
    vector<int> moves;
    coord2 c(cube);
    bool run = true;
    while(run){
        run = false;
        int dist = (arr.get(c.index()) + 2) % 3;
        //printf("c = %d, dist = %d, \n", c.index(), arr.get(c.index()));
        for(int i = 0; i < 10; ++i){
            coord2 d = c.neigh(i);
            if(arr.get(d.index()) == dist){
                //printf("move %d\n", indeces[i]);
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
    symmetric_pos = symmetric();
    printf("symmetric positions: %d\n", symmetric_pos.count());
    corner_sign = cornersign();
    printf("all calculated\n");

    int depth = 11;
    table arr(2768 * 483840);
    pruning_table(arr, 12);
    forward_search(arr, 13, 15);
    backward_search(arr, 16, 18);
    //io::write("stage22.bin", arr.bits, (arr.len + 3) / 4);
    arr.stats();

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
    //*
    bool allok = true;
    for(int i = 0; i < 50000; ++i){
        Cube cube;
        vector<int> shuf = cube.shuffle(indeces, 50);
        Cube copy = cube;
        //cube.print();
        vector<int> moves = solve(copy, arr);
        for(int i : moves) copy *= Cube::moves[indeces[i]];
        //cube.print();
        bool flag = copy.is_solved();
        /*
        if(!flag){
            cube.print();
            printf("shuffle: ");
            for(int m : shuf) printf("%d, ", m);
            printf("\nsolution: ");
            for(int m : moves) printf("%d, ", indeces[m]);
            printf("\n");
            Cube last_move = Cube::moves[shuf.back()];
            copy *= last_move * last_move * last_move;
            vector<int> moves = solve(copy, arr);
            for(int i : moves) copy *= Cube::moves[indeces[i]];
            for(int m : moves) printf("%d, ", indeces[m]);
            copy.print();
        }
        */
        allok &= flag;
    }
    printf("ok: %d\n", allok);
    //*/
}