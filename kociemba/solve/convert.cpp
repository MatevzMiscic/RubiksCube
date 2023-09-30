#include <iostream>
#include <array>
#include "../Settings.h"
#include "../Cube.h"
#include "convert.h"

//using namespace std;


struct pair{
    ubyte a, b;
    pair(ubyte a, ubyte b) : a(a), b(b) {}
    void turn(){
        ubyte c = a;
        a = b;
        b = c;
    }
};

struct triple{
    ubyte a, b, c;
    triple(ubyte a, ubyte b, ubyte c) : a(a), b(b), c(c) {}
    void turn(){
        ubyte d = a;
        a = b;
        b = c;
        c = d;
    }
};

std::array<pair, 12> edge_to_pair = {pair(46, 16), pair(50, 25), pair(52, 34), pair(48, 7), pair(5, 12), pair(21, 14), pair(23, 30), pair(3, 32), pair(43, 10), pair(41, 19), pair(37, 28), pair(39, 1)};

std::array<triple, 8> corner_to_triple = {triple(45, 8, 15), triple(47, 17, 24), triple(53, 26, 33), triple(51, 35, 6), triple(42, 9, 2), triple(44, 18, 11), triple(38, 27, 20), triple(36, 0, 29)};

int find_edge(pair& col){
    for(int i = 0; i < 12; ++i){
        pair& edge = edge_to_pair[i];
        if(col.a == edge.a / 9 && col.b == edge.b / 9){
            return i;
        }
    }
    return -1;
}

int find_corner(triple& col){
    for(int i = 0; i < 8; ++i){
        triple& corner = corner_to_triple[i];
        if(col.a == corner.a / 9 && col.b == corner.b / 9 && col.c == corner.c / 9){
            return i;
        }
    }
    return -1;
}

bool cnv::to_cube(std::array<ubyte, 54>& colors, Cube& cube){
    std::array<ubyte, 6> map;
    for(int i = 0; i < 6; ++i) map[colors[4 + 9*i]] = i;
    for(int i = 0; i < 54; ++i) colors[i] = map[colors[i]];
    for(int i = 0; i < 8; ++i){
        triple& tri = corner_to_triple[i];
        triple col(colors[tri.a], colors[tri.b], colors[tri.c]);
        int cor;
        for(int j = 0; j < 3; ++j){
            cor = find_corner(col);
            if(cor >= 0){
                cube.corner[i] = cor;
                cube.twist[i] = j;
                break;
            }
            col.turn();
        }
        if(cor < 0) return false;
    }
    for(int i = 0; i < 12; ++i){
        pair& p = edge_to_pair[i];
        pair col(colors[p.a], colors[p.b]);
        int edg;
        for(int j = 0; j < 2; ++j){
            edg = find_edge(col);
            if(edg >= 0){
                cube.edge[i] = edg;
                cube.flip[i] = j;
                break;
            }
            col.turn();
        }
        if(edg < 0) return false;
    }
    return true;
}



int main1(){
    std::array<ubyte, 54> colors = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 
        1, 1, 5, 1, 1, 5, 1, 1, 5, 
        2, 2, 2, 2, 2, 2, 2, 2, 2, 
        4, 3, 3, 4, 3, 3, 4, 3, 3, 
        4, 4, 1, 4, 4, 1, 4, 4, 1, 
        5, 5, 3, 5, 5, 3, 5, 5, 3
    };
    Cube cube;
    cnv::to_cube(colors, cube);
    Cube::R.print();
    cube.print();
    return 0;
}