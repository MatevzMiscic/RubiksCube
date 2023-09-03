#include <iostream>
#include "Cube.h"
#include "math/Permutation.h"

using namespace std;





Cube::Cube(){
    for(int i = 0; i < 8; ++i){
        corner[i] = i;
    }
    twist.fill(0);
    for(int i = 0; i < 12; ++i){
        edge[i] = i;
    }
    flip.fill(0);
}

Cube::Cube(const array<byte, 8>& corner, const array<byte, 8>& twist, const array<byte, 12>& edge, const array<byte, 12>& flip){
    this->corner = corner;
    this->twist = twist;
    this->edge = edge;
    this->flip = flip;
}





// multiplication table for group D6
array<byte, 36> dihedral = {
    0, 1, 2, 3, 4, 5, 
    1, 2, 0, 5, 3, 4, 
    2, 0, 1, 4, 5, 3, 
    3, 4, 5, 0, 1, 2, 
    4, 5, 3, 2, 0, 1, 
    5, 3, 4, 1, 2, 0
};
// inverses of elements of group D6
array<byte, 6> dihedralinv = {0, 2, 1, 3, 4, 5};


// operations in 'is replaced by' representation
// this representation is more suitable for move tables, because what happens to twist or flip only depends on the move and twist or flip respectively
void Cube::operator*=(const Cube& cube){
    array<byte, 8> corner;
    array<byte, 8> twist;
    array<byte, 12> edge;
    array<byte, 12> flip;
    for(int i = 0; i < 8; ++i){
        // twist[i] = (cube.twist[i] + this->twist[cube.corner[i]]) % 3;
        twist[i] = dihedral[6 * this->twist[cube.corner[i]] + cube.twist[i]];
        corner[i] = this->corner[cube.corner[i]];
    }
    for(int i = 0; i < 12; ++i){
        flip[i] = (cube.flip[i] + this->flip[cube.edge[i]]) % 2;
        edge[i] = this->edge[cube.edge[i]];
    }
    this->corner = corner;
    this->twist = twist;
    this->edge = edge;
    this->flip = flip;
}

Cube Cube::operator*(const Cube& cube){
    array<byte, 8> corner;
    array<byte, 8> twist;
    array<byte, 12> edge;
    array<byte, 12> flip;
    for(int i = 0; i < 8; ++i){
        //twist[i] = (cube.twist[i] + this->twist[cube.corner[i]]) % 3;
        twist[i] = dihedral[6 * this->twist[cube.corner[i]] + cube.twist[i]];
        corner[i] = this->corner[cube.corner[i]];
    }
    for(int i = 0; i < 12; ++i){
        flip[i] = (cube.flip[i] + this->flip[cube.edge[i]]) % 2;
        edge[i] = this->edge[cube.edge[i]];
    }
    return Cube(corner, twist, edge, flip);
}

// returns inverse of a cube
Cube Cube::inv(){
    array<byte, 8> corner;
    array<byte, 8> twist;
    array<byte, 12> edge;
    array<byte, 12> flip;
    for(int i = 0; i < 8; ++i){
        // twist[this->corner[i]] = (3 -  this->twist[i]) % 3;
        twist[this->corner[i]] = dihedralinv[this->twist[i]];
        corner[this->corner[i]] = i;
    }
    for(int i = 0; i < 12; ++i){
        flip[this->edge[i]] = this->flip[i];
        edge[this->edge[i]] = i;
    }
    return Cube(corner, twist, edge, flip);
}






// returns corner_coord
int Cube::corner_coord(){
    return perm::rank(corner);
}

// returns twist_coord in 'is replaced by' representation
int Cube::twist_coord(){
    int coord = 0;
    for(int i = 0; i < 7; ++i){
        coord = 3*coord + twist[i];
    }
    return coord;
}

// returns edge_coord
int Cube::edge_coord(){
    return perm::rank(edge);
}

// returns flip_coord in 'is replaced by' representation
int Cube::flip_coord(){
    int coord = 0;
    for(int i = 0; i < 11; ++i){
        coord = (coord << 1) | flip[i];
    }
    return coord;
}





// define moves in half turn metric in 'is replaced by' representation
Cube Cube::F(array<byte, 8>{1, 5, 2, 3, 0, 4, 6, 7}, array<byte, 8>{1, 2, 0, 0, 2, 1, 0, 0}, array<byte, 12>{5, 1, 2, 3, 0, 8, 6, 7, 4, 9, 10, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::B(array<byte, 8>{0, 1, 3, 7, 4, 5, 2, 6}, array<byte, 8>{0, 0, 1, 2, 0, 0, 2, 1}, array<byte, 12>{0, 1, 7, 3, 4, 5, 2, 10, 8, 9, 6, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::U(array<byte, 8>{0, 1, 2, 3, 5, 6, 7, 4}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::D(array<byte, 8>{3, 0, 1, 2, 4, 5, 6, 7}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
//Cube Cube::L3(array<byte, 8>{3, 1, 2, 7, 0, 5, 6, 4}, array<byte, 8>{2, 0, 0, 1, 1, 0, 0, 2}, array<byte, 12>{0, 1, 2, 7, 3, 5, 6, 11, 8, 9, 10, 4}, array<byte, 12>{0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1});
Cube Cube::L(array<byte, 8>{4, 1, 2, 0, 7, 5, 6, 3}, array<byte, 8>{2, 0, 0, 1, 1, 0, 0, 2}, array<byte, 12>{0, 1, 2, 4, 11, 5, 6, 3, 8, 9, 10, 7}, array<byte, 12>{0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1});
Cube Cube::R(array<byte, 8>{0, 2, 6, 3, 4, 1, 5, 7}, array<byte, 8>{0, 1, 2, 0, 0, 2, 1, 0}, array<byte, 12>{0, 6, 2, 3, 4, 1, 9, 7, 8, 5, 10, 11}, array<byte, 12>{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0});

Cube Cube::F2 = Cube::F * Cube::F;
Cube Cube::B2 = Cube::B * Cube::B;
Cube Cube::U2 = Cube::U * Cube::U;
Cube Cube::D2 = Cube::D * Cube::D;
Cube Cube::L2 = Cube::L * Cube::L;
Cube Cube::R2 = Cube::R * Cube::R;

Cube Cube::F3 = Cube::F2 * Cube::F;
Cube Cube::B3 = Cube::B2 * Cube::B;
Cube Cube::U3 = Cube::U2 * Cube::U;
Cube Cube::D3 = Cube::D2 * Cube::D;
Cube Cube::L3 = Cube::L2 * Cube::L;
Cube Cube::R3 = Cube::R2 * Cube::R;

array<Cube, 18> Cube::moves = {
    Cube::F, Cube::B, Cube::U, Cube::D, Cube::L, Cube::R, 
    Cube::F2, Cube::B2, Cube::U2, Cube::D2, Cube::L2, Cube::R2, 
    Cube::F3, Cube::B3, Cube::U3, Cube::D3, Cube::L3, Cube::R3
};





// prints the cube
void Cube::print(){
    printf("(");
    for(byte c : corner) printf("%d ", c);
    printf("), (");
    for(byte c : twist) printf("%d ", c);
    printf("), (");
    for(byte c : edge) printf("%d ", c);
    printf("), (");
    for(byte c : flip) printf("%d ", c);
    printf(")\n");
}