#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Cube.h"
#include "../math/Permutation.h"
#include "../moves/moves.h"
#include "../io/io.h"
#include "../Settings.h"
#include "../preprocess/table.h"
#include "convert.h"



using namespace std;



vector<ushort> twist_mt;
vector<ushort> flip_mt;
vector<ushort> slice_mt;

table stage1_pt(2217093120);


struct coord1{
    ushort twist;
    ushort flip;
    ushort slice;

    coord1(Cube& cube){
        twist = cube.twist_coord();
        flip = cube.flip_coord();
        slice = cube.slice_coord();
    }

    coord1(int twist, int flip, int slice){
        this->twist = twist;
        this->flip = flip;
        this->slice = slice;
    }

    inline bool is_solved(){
        return twist == 0 && flip == 0 && slice == 69;
    }

    inline uint index(){
        return 2187 * ((slice << 11) | flip) + twist;
    }

    inline coord1 neigh(int i){
        return coord1(twist_mt[18*twist + i], flip_mt[18*flip + i], slice_mt[18*slice + i]);
    }

    void print(){
        printf("idx: %d, slice: %d, twist: %d, flip: %d\n", index(), slice, flip, twist);
    }
};


vector<ubyte> stage1(Cube& cube){
    vector<ubyte> moves;
    coord1 c(cube);
    bool run = true;
    while(run){
        run = false;
        int dist = (stage1_pt.get(c.index()) + 2) % 3;
        //printf("c = %d, dist = %d, sl: %d, fl; %d, tw: %d\n", c.index(), stage1_pt.get(c.index()), c.slice(), c.flip(), c.twist());
        for(int i = 0; i < 18; ++i){
            coord1 d = c.neigh(i);
            //printf("neigh: %d, %d\n", d.index(), stage1_pt.get(d.index()));
            if(stage1_pt.get(d.index()) == dist){
                //printf("move %d\n", i);
                moves.push_back(i);
                run = true;
                c = d;
                break;
            }
        }
    }
    return moves;
}






vector<ushort> st;
vector<ushort> rt;
vector<ushort> sym_to_raw;
vector<ushort> raw_to_sym;

vector<ushort> corner_mt;
vector<uint> layer_mt[2];
vector<uint> corner_ist[2];

vector<int> inv = {0, 1, 6, 3, 4, 5, 2, 7, 8, 9, 10, 15, 12, 13, 14, 11};
array<uint, 10> move_sign;

table stage2_pt(2768 * 483840);


inline int out(int a){
    if(a < 4) return a;
    return a - 4;
}
int layers_compressed_coord(Cube& cube){
    array<byte, 4> slice;
    array<byte, 8> ud;
    for(int i = 0; i < 4; ++i) ud[i] = out(cube.edge[i]);
    for(int i = 0; i < 4; ++i) slice[i] = cube.edge[i + 4] - 4;
    for(int i = 8; i < 12; ++i) ud[i - 4] = out(cube.edge[i]);
    return 12 * perm::rank(ud) + perm::rank_sign(slice, perm::sign(slice));
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
        return 483840 * (corner >> 4) + corner_ist[sign][(layer << 4) | (corner & 15)];
    }

    inline coord2 neigh(int i){
        return coord2(corner_mt[10 * corner + i], layer_mt[sign][10 * layer + i], move_sign[i] ^ sign);
    }

    inline void print() const {
        printf("corner: %d, layer %d, sign: %d\n", corner >> 4, layer, sign);
    }
};

vector<ubyte> stage2(Cube& cube){
    array<ubyte, 10> gens = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    vector<ubyte> moves;
    coord2 c(cube);
    bool run = true;
    while(run){
        run = false;
        int dist = (stage2_pt.get(c.index()) + 2) % 3;
        //printf("c = %d, dist = %d, \n", c.index(), stage2_pt.get(c.index()));
        for(int i = 0; i < 10; ++i){
            coord2 d = c.neigh(i);
            //printf("   %d; idx: %d, dist: %d\n", i, d.index(), stage2_pt.get(d.index()));
            if(stage2_pt.get(d.index()) == dist){
                moves.push_back(gens[i]);
                run = true;
                c = d;
                break;
            }
        }
    }
    return moves;
}



vector<ubyte> solve(Cube cube){
    vector<ubyte> first = stage1(cube);
    for(int move : first) cube *= Cube::moves[move];
    //cube.print();
    vector<ubyte> second = stage2(cube);
    first.insert(first.end(), second.begin(), second.end());
    return first;
}







void search(vector<vector<ubyte>>& solutions, vector<ubyte>& moves, coord1& coord, int slack){
    if(coord.is_solved() && slack == 0){
        vector<ubyte> copy = moves;
        solutions.push_back(copy);
        return;
    }
    int dist = stage1_pt.get(coord.index());
    for(int i = 0; i < 18; ++i){
        coord1 next = coord.neigh(i);
        int change = (stage1_pt.get(next.index()) + 4 - dist) % 3;
        if(change <= slack){
            moves.push_back(i);
            search(solutions, moves, next, slack - change);
            moves.pop_back();
        }
    }
}

vector<vector<ubyte>> all_solutions(Cube& cube, int slack){
    vector<vector<ubyte>> solutions;
    vector<ubyte> moves;
    coord1 coord(cube);
    search(solutions, moves, coord, slack);
    return solutions;
}

vector<ubyte> best_solve(Cube& cube){
    int final_slack = 0;
    int len = 31;
    vector<ubyte> solution;
    for(int slack = 0; slack < 3; ++slack){
        vector<vector<ubyte>> sols = all_solutions(cube, slack);
        for(vector<ubyte>& first : sols){
            Cube copy = cube;
            for(byte move : first){
                copy *= Cube::moves[move];
            }
            vector<ubyte> second = stage2(copy);
            if(first.size() + second.size() < len){
                len = first.size() + second.size();
                solution = first;
                solution.insert(solution.end(), second.begin(), second.end());
                final_slack = slack;
            }
        }
    }
    printf("final slack: %d\n", final_slack);
    return solution;
}


int main(){
    bool read = true;

    cmb::calcbin();
    twist_mt = move::twist_movetable();
    flip_mt = move::flip_movetable();
    slice_mt = move::slice_movetable();
    read &= io::read("../preprocess/stage1.bin", stage1_pt.bits, (stage1_pt.len + 3) / 4);

    array<int, 10> gens = {2, 3, 6, 7, 8, 9, 10, 11, 14, 15};
    corner_mt = move::corner_sym_movetable();
    move::layers_compressed_movetable(layer_mt[0], layer_mt[1]);
    corner_ist[0].assign(16*483840, 0);
    corner_ist[1].assign(16*483840, 0);
    read &= io::read("../symmetry/layer_even.sym", (int*)(&corner_ist[0][0]), 4*corner_ist[0].size());
    read &= io::read("../symmetry/layer_odd.sym", (int*)(&corner_ist[1][0]), 4*corner_ist[1].size());
    read &= io::read("../preprocess/stage2.bin", stage2_pt.bits, (stage2_pt.len + 3) / 4);
    st = sym::corner_st();
    rt = sym::corner_rt(st);
    sym::corner_conversion(sym_to_raw, raw_to_sym);
    sym::compute();
    for(int i = 0; i < 10; ++i) move_sign[i] = perm::sign(Cube::moves[gens[i]].corner);

    printf("tables read: %d\n", read);

    /*
    int runs = 50000;
    uint sum = 0;
    uint min = 100000;
    uint max = 0;
    bool ok = true;
    for(int i = 0; i < runs; ++i){
        Cube cube;
        cube.shuffle();
        Cube copy = cube;
        vector<int> moves = solve(copy);
        for(int move : moves){
            copy *= Cube::moves[move];
        }
        ok = ok && copy.is_solved();
        if(!ok){
            cube.print();
            copy.print();
            break;
        }
        sum += moves.size();
        if(moves.size() > max) max = moves.size();
        if(moves.size() < min) min = moves.size();
    }
    printf("all ok: %d\n", ok);
    printf("avg moves: %f\n", ((double)sum) / runs);
    printf("max moves: %d\n", max);
    printf("min moves: %d\n", min);
    */
    array<string, 18> names = {
        "F", "B", "U", "D", "L", "R", 
        "F2", "B2", "U2", "D2", "L2", "R2", 
        "F3", "B3", "U3", "D3", "L3", "R3"
    };
    array<ubyte, 54> colors1 = {
        0, 0, 0, 0, 0, 0, 2, 0, 5, 
        1, 1, 1, 1, 1, 1, 1, 2, 5, 
        2, 2, 2, 2, 2, 2, 3, 1, 0, 
        3, 3, 3, 3, 3, 3, 1, 3, 5, 
        4, 4, 4, 4, 4, 4, 4, 4, 4, 
        2, 5, 0, 5, 5, 5, 3, 5, 5
    };
    array<ubyte, 54> colors = {
        4, 4, 2, 1, 0, 2, 5, 1, 5,
        3, 0, 2, 5, 1, 4, 1, 0, 1,
        5, 4, 0, 3, 2, 5, 0, 3, 0,
        4, 5, 2, 3, 3, 5, 1, 2, 0,
        1, 0, 3, 1, 4, 0, 4, 3, 3,
        2, 1, 4, 2, 5, 2, 3, 4, 5
    };
    Cube cube;
    printf("converted: %d\n", cnv::to_cube(colors, cube));
    Cube copy = cube;
    vector<ubyte> moves = best_solve(copy);
    for(int move : moves){
        copy *= Cube::moves[move];
        cout << names[move] << ", ";
    }
    copy.print();

    int sum = 0;
    int min = 100000;
    int max = 0;
    int errors = 0;
    printf("solutions:\n");
    vector<vector<ubyte>> sols = all_solutions(cube, 1);
    for(vector<ubyte>& sol : sols){
        Cube copy = cube;
        for(byte move : sol){
            //printf("%d, ", move);
            copy *= Cube::moves[move];
        }
        if(!copy.in_subgroup()){
            errors += 1;
        }else{
            int num = stage2(copy).size();
            sum += num;
            if(num > max) max = num;
            if(num < min) min = num;
        }
        //printf("\n");
    }
    printf("errors: %d\n", errors);
    printf("avg moves: %f\n", ((double)sum) / sols.size());
    printf("max moves: %d\n", max);
    printf("min moves: %d\n", min);
}