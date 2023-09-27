#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "../Cube.h"
#include "../math/Permutation.h"
#include "../moves/moves.h"
#include "../io/io.h"
#include "../Settings.h"
#include "../preprocess/table.h"
#include "convert.h"
#include "solver.h"



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

array<uint, 10> move_sign;

table stage2_pt(2768 * 483840);



bool solver::init(){
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

    return read;
}




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





vector<ubyte> solver::solve(Cube cube){
    vector<ubyte> first = stage1(cube);
    for(int move : first) cube *= Cube::moves[move];
    vector<ubyte> second = stage2(cube);
    first.insert(first.end(), second.begin(), second.end());
    return first;
}










array<vector<ubyte>, 7> next_moves = {
    vector<ubyte>{1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17}, 
    vector<ubyte>{2, 3, 4, 5, 8, 9, 10, 11, 14, 15, 16, 17}, 
    vector<ubyte>{0, 1, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 15, 16, 17}, 
    vector<ubyte>{0, 1, 4, 5, 6, 7, 10, 11, 12, 13, 16, 17}, 
    vector<ubyte>{0, 1, 2, 3, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 17}, 
    vector<ubyte>{0, 1, 2, 3, 6, 7, 8, 9, 12, 13, 14, 15}, 
    vector<ubyte>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 
};
array<bool, 18> subgroup_gen = {
    false, false, true, true, false, false, 
    true, true, true, true, true, true, 
    false, false, true, true, false, false, 
};

void search(vector<vector<ubyte>>& solutions, vector<ubyte>& moves, coord1& coord, int slack){
    if(coord.is_solved()){
        if(slack == 0 && (moves.empty() || !subgroup_gen[moves.back()])){
            vector<ubyte> copy = moves;
            solutions.push_back(copy);
        }
        return;
    }
    int dist = stage1_pt.get(coord.index());
    int last = 6;
    if(!moves.empty()) last = moves.back() % 6;
    for(ubyte i : next_moves[last]){
        coord1 next = coord.neigh(i);
        int change = (stage1_pt.get(next.index()) + 4 - dist) % 3;
        if(change <= slack){
            moves.push_back(i);
            search(solutions, moves, next, slack - change);
            moves.pop_back();
        }
    }
}

void search2(vector<ubyte>& solution, uint& len, vector<ubyte>& moves, Cube cube, coord1& coord, int slack){
    if(coord.is_solved()){
        if(slack == 0 && (moves.empty() || !subgroup_gen[moves.back()])){
            vector<ubyte> second = stage2(cube);
            if(moves.size() + second.size() < len){
                len = moves.size() + second.size();
                solution = moves;
                solution.insert(solution.end(), second.begin(), second.end());
            }
        }
        return;
    }
    int dist = stage1_pt.get(coord.index());
    int last = 6;
    if(!moves.empty()) last = moves.back() % 6;
    for(ubyte i : next_moves[last]){
        coord1 next = coord.neigh(i);
        int change = (stage1_pt.get(next.index()) + 4 - dist) % 3;
        if(change <= slack){
            moves.push_back(i);
            search2(solution, len, moves, cube * Cube::moves[i], next, slack - change);
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
    //int final_slack = 0;
    int len = 31;
    vector<ubyte> solution;
    for(int slack = 0; slack < 3; ++slack){
        vector<vector<ubyte>> sols = all_solutions(cube, slack);
        //printf("slack: %d, solutions: %d\n", slack, sols.size());
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
                //final_slack = slack;
            }
        }
    }
    //printf("final slack: %d\n", final_slack);
    return solution;
}

vector<ubyte> three_axis_solve(Cube& cube){
    vector<ubyte> out = best_solve(cube);
    cube = sym::thirdturn * cube * sym::thirdturn.inv();
    vector<ubyte> solution = best_solve(cube);
    if(solution.size() < out.size()) out = solution;
    cube = sym::thirdturn * cube * sym::thirdturn.inv();
    solution = best_solve(cube);
    if(solution.size() < out.size()) out = solution;
    cube = cube.inv();
    solution = best_solve(cube);
    if(solution.size() < out.size()) out = solution;
    cube = sym::thirdturn * cube * sym::thirdturn.inv();
    solution = best_solve(cube);
    if(solution.size() < out.size()) out = solution;
    cube = sym::thirdturn * cube * sym::thirdturn.inv();
    solution = best_solve(cube);
    if(solution.size() < out.size()) out = solution;
    return out;
}

void solve_wrapper(Cube& cube, vector<ubyte>& solution){
    //solution = best_solve(cube);
    uint len = 31;
    vector<ubyte> moves;
    coord1 coord(cube);
    for(uint i = 0; i < 3; ++i){
        search2(solution, len, moves, cube, coord, i);
    }
}

array<ubyte, 18> inverses = {12, 13, 14, 15, 16, 17, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5};

array<array<ubyte, 18>, 6> post_process = {
    array<ubyte, 18>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}, 
    array<ubyte, 18>{2, 3, 5, 4, 1, 0, 8, 9, 11, 10, 7, 6, 14, 15, 17, 16, 13, 12}, 
    array<ubyte, 18>{5, 4, 0, 1, 3, 2, 11, 10, 6, 7, 9, 8, 17, 16, 12, 13, 15, 14}, 
    array<ubyte, 18>{12, 13, 14, 15, 16, 17, 6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5}, 
    array<ubyte, 18>{14, 15, 17, 16, 13, 12, 8, 9, 11, 10, 7, 6, 2, 3, 5, 4, 1, 0}, 
    array<ubyte, 18>{17, 16, 12, 13, 15, 14, 11, 10, 6, 7, 9, 8, 5, 4, 0, 1, 3, 2}
};

vector<ubyte> parallel_solve(Cube& cube){
    Cube inv = cube.inv();
    array<Cube, 6> cubes = {
        cube, sym::thirdturn * cube * sym::thirdturn.inv(), sym::thirdturn.inv() * cube * sym::thirdturn, 
        inv, sym::thirdturn * inv * sym::thirdturn.inv(), sym::thirdturn.inv() * inv * sym::thirdturn, 
    };
    array<vector<ubyte>, 6> solutions;
    array<thread, 6> threads;
    for(int i = 0; i < 6; ++i){
        threads[i] = thread(solve_wrapper, ref(cubes[i]), ref(solutions[i]));
    }
    for(int i = 0; i < 6; ++i){
        threads[i].join();
    }
    int idx = 0;
    for(int i = 1; i < 6; ++i){
        if(solutions[i].size() < solutions[idx].size()) idx = i;
    }
    if(idx >= 3){
        reverse(solutions[idx].begin(), solutions[idx].end());
    }
    for(int i = 0; i < solutions[idx].size(); ++i) solutions[idx][i] = post_process[idx][solutions[idx][i]];
    return solutions[idx];
}

void timelimit_search(vector<ubyte>& solution, int& len, vector<ubyte>& moves, Cube cube, coord1& coord, int slack, chrono::steady_clock::time_point& end){
    if(chrono::steady_clock::now() >= end && len < 31){
        //printf("%d\n", chrono::duration_cast<chrono::microseconds>(end - chrono::steady_clock::now()).count());
        return;
    }
    if(coord.is_solved()){
        if(slack == 0 && (moves.empty() || !subgroup_gen[moves.back()])){
            vector<ubyte> second = stage2(cube);
            if(moves.size() + second.size() < len){
                len = moves.size() + second.size();
                solution = moves;
                solution.insert(solution.end(), second.begin(), second.end());
            }
        }
        return;
    }
    int dist = stage1_pt.get(coord.index());
    int last = 6;
    if(!moves.empty()) last = moves.back() % 6;
    for(ubyte i : next_moves[last]){
        coord1 next = coord.neigh(i);
        int change = (stage1_pt.get(next.index()) + 4 - dist) % 3;
        if(change <= slack){
            moves.push_back(i);
            timelimit_search(solution, len, moves, cube * Cube::moves[i], next, slack - change, end);
            moves.pop_back();
        }
    }
}

void timelimit_wrapper(Cube& cube, vector<ubyte>& solution, chrono::steady_clock::time_point& end){
    //solution = best_solve(cube);
    int len = 31;
    vector<ubyte> moves;
    coord1 coord(cube);
    for(int i = 0; i < 6; ++i){
        timelimit_search(solution, len, moves, cube, coord, i, end);
        if(chrono::steady_clock::now() > end){
            //printf("last stage %d, timeout :%d\n", i, -chrono::duration_cast<chrono::microseconds>(end - chrono::steady_clock::now()).count());
            break;
        }
    }
}


vector<ubyte> solver::timelimit_solve(const Cube& cube, int ms){
    Cube inv = cube.inv();
    array<Cube, 6> cubes = {
        cube, sym::thirdturn * cube * sym::thirdturn.inv(), sym::thirdturn.inv() * cube * sym::thirdturn, 
        inv, sym::thirdturn * inv * sym::thirdturn.inv(), sym::thirdturn.inv() * inv * sym::thirdturn, 
    };
    array<vector<ubyte>, 6> solutions;
    array<thread, 6> threads;
    chrono::steady_clock::time_point end = chrono::steady_clock::now() + chrono::milliseconds(ms);
    for(int i = 0; i < 6; ++i){
        threads[i] = thread(timelimit_wrapper, ref(cubes[i]), ref(solutions[i]), ref(end));
    }
    for(int i = 0; i < 6; ++i){
        threads[i].join();
    }
    int idx = 0;
    for(int i = 1; i < 6; ++i){
        if(solutions[i].size() < solutions[idx].size()) idx = i;
    }
    if(idx >= 3){
        reverse(solutions[idx].begin(), solutions[idx].end());
    }
    for(int i = 0; i < solutions[idx].size(); ++i) solutions[idx][i] = post_process[idx][solutions[idx][i]];
    //printf("index: %d\n", idx);
    return solutions[idx];
}



void movelimit_search(vector<ubyte>& solution, int& len, vector<ubyte>& moves, Cube cube, coord1& coord, int slack, ubyte movelimit){
    if(coord.is_solved()){
        if(slack == 0 && (moves.empty() || !subgroup_gen[moves.back()])){
            vector<ubyte> second = stage2(cube);
            if(moves.size() + second.size() < len){
                len = moves.size() + second.size();
                solution = moves;
                solution.insert(solution.end(), second.begin(), second.end());
            }
        }
        return;
    }
    int dist = stage1_pt.get(coord.index());
    int last = 6;
    if(!moves.empty()) last = moves.back() % 6;
    for(ubyte i : next_moves[last]){
        coord1 next = coord.neigh(i);
        int change = (stage1_pt.get(next.index()) + 4 - dist) % 3;
        if(change <= slack){
            moves.push_back(i);
            movelimit_search(solution, len, moves, cube * Cube::moves[i], next, slack - change, movelimit);
            moves.pop_back();
            if(len <= movelimit) return;
        }
    }
}

void movelimit_wrapper(Cube& cube, vector<ubyte>& solution, ubyte movelimit){
    //solution = best_solve(cube);
    int len = 31;
    vector<ubyte> moves;
    coord1 coord(cube);
    for(int i = 0; i < 6; ++i){
        movelimit_search(solution, len, moves, cube, coord, i, movelimit);
        if(len <= movelimit){
            break;
        }
    }
}


vector<ubyte> solver::movelimit_solve(const Cube& cube, ubyte movelimit){
    Cube inv = cube.inv();
    array<Cube, 6> cubes = {
        cube, sym::thirdturn * cube * sym::thirdturn.inv(), sym::thirdturn.inv() * cube * sym::thirdturn, 
        inv, sym::thirdturn * inv * sym::thirdturn.inv(), sym::thirdturn.inv() * inv * sym::thirdturn, 
    };
    array<vector<ubyte>, 6> solutions;
    array<thread, 6> threads;
    for(int i = 0; i < 6; ++i){
        threads[i] = thread(movelimit_wrapper, ref(cubes[i]), ref(solutions[i]), movelimit);
    }
    for(int i = 0; i < 6; ++i){
        threads[i].join();
    }
    int idx = 0;
    for(int i = 1; i < 6; ++i){
        if(solutions[i].size() < solutions[idx].size()) idx = i;
    }
    if(idx >= 3){
        reverse(solutions[idx].begin(), solutions[idx].end());
    }
    for(int i = 0; i < solutions[idx].size(); ++i) solutions[idx][i] = post_process[idx][solutions[idx][i]];
    //printf("index: %d\n", idx);
    return solutions[idx];
}






int recognise(const Cube& cube){
    for(int i = 0; i < 18; ++i){
        if(cube == Cube::moves[i]) return i;
    }
    return -1;
}

int main111(){
    bool read = true;
    /*
    for(int i = 0; i < 18; ++i){
        Cube tt = sym::thirdturn;
        Cube itt = sym::thirdturn.inv();
        printf("%d, ", recognise(itt * Cube::moves[i] * tt));
    }
    printf("\n");
    for(int i = 0; i < 18; ++i){
        Cube tt = sym::thirdturn;
        Cube itt = sym::thirdturn.inv();
        printf("%d, ", recognise(tt * Cube::moves[i] * itt));
    }
    printf("\n");
    for(int i = 0; i < 18; ++i){
        Cube tt = sym::thirdturn;
        Cube itt = sym::thirdturn.inv();
        printf("%d, ", inverses[recognise(itt * Cube::moves[i] * tt)]);
    }
    printf("\n");
    for(int i = 0; i < 18; ++i){
        Cube tt = sym::thirdturn;
        Cube itt = sym::thirdturn.inv();
        printf("%d, ", inverses[recognise(tt * Cube::moves[i] * itt)]);
    }
    printf("\n");
    */

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

    
    int runs = 100;
    ll total_time = 0;
    uint sum = 0;
    uint min = 100000;
    uint max = 0;
    bool ok = true;
    for(int i = 0; i < runs; ++i){
        Cube cube;
        cube.shuffle();
        Cube copy = cube;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        //vector<ubyte> moves = solver::three_axis_solve(copy);
        //vector<ubyte> moves = solver::parallel_solve(copy);
        //vector<ubyte> moves = solver::timelimit_solve(copy, 100);
        vector<ubyte> moves = solver::movelimit_solve(copy, 22);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        ll time = (chrono::duration_cast<chrono::microseconds>(end - begin).count());
        total_time += time;
        for(int move : moves){
            copy *= Cube::moves[move];
        }
        ok = ok && copy.is_solved();
        /*
        if(!ok){
            cube.print();
            copy.print();
            break;
        }
        */
        sum += moves.size();
        if(moves.size() > max) max = moves.size();
        if(moves.size() < min) min = moves.size();
    }
    printf("all ok: %d\n", ok);
    printf("avg moves: %f\n", ((double)sum) / runs);
    printf("max moves: %d\n", max);
    printf("min moves: %d\n", min);
    printf("average time: %f\n", total_time / (1000000.0 * runs));

    /*
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
    */
   return 0;
}