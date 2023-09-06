#include "moves.h"


int main(){
    /*
    std::array<short, 18*2187> movetable = twist_movetable();
    int a = 4, b = 5;
    int c1 = (Cube::moves[a] * Cube::moves[b]).twist_coord();
    int c2 = movetable[18 * Cube::moves[a].twist_coord() + b];
    //(Cube::moves[a] * Cube::moves[b]).print();
    //printf("%d\n", Cube::moves[a].twist_coord());
    printf("%d, %d\n", c1, c2);
    */
    std::array<int, 100000> reprs = represent_table();
}