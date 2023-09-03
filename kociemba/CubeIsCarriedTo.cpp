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


// operations in 'is carried to' representation
void Cube::operator*=(const Cube& cube){
    for(int i = 0; i < 8; ++i){
        twist[i] = (twist[i] + cube.twist[corner[i]]) % 3;
        corner[i] = cube.corner[corner[i]];
    }
    for(int i = 0; i < 12; ++i){
        flip[i] = (flip[i] + cube.flip[edge[i]]) % 2;
        edge[i] = cube.edge[edge[i]];
    }
}

Cube Cube::operator*(const Cube& cube){
    array<byte, 8> corner;
    array<byte, 8> twist;
    array<byte, 12> edge;
    array<byte, 12> flip;
    for(int i = 0; i < 8; ++i){
        twist[i] = (this->twist[i] + cube.twist[this->corner[i]]) % 3;
        corner[i] = cube.corner[this->corner[i]];
    }
    for(int i = 0; i < 12; ++i){
        flip[i] = (this->flip[i] + cube.flip[this->edge[i]]) % 2;
        edge[i] = cube.edge[this->edge[i]];
    }
    return Cube(corner, twist, edge, flip);
}



// returns corner_coord
int Cube::corner_coord(){
    return perm::rank(corner);
}

// returns twist_coord in 'is carried to' representation
int Cube::twist_coord(){
    array<byte, 8> twist_pos;
    for(int i = 0; i < 8; ++i){
        twist_pos[corner[i]] = twist[i];
    }
    int coord = 0;
    for(int i = 0; i < 7; ++i){
        coord = 3*coord + twist_pos[i];
    }
    return coord;
}

// returns edge_coord
int Cube::edge_coord(){
    return perm::rank(edge);
}

// returns flip_coord in 'is carried to' representation
int Cube::flip_coord(){
    int coord = 0;
    for(int i = 0; i < 11; ++i){
        coord |= flip[i] << edge[i];
    }
    return coord;
}




// define moves in half turn metric in 'is carried to' representation
Cube Cube::F(array<byte, 8>{4, 0, 2, 3, 5, 1, 6, 7}, array<byte, 8>{2, 1, 0, 0, 1, 2, 0, 0}, array<byte, 12>{4, 1, 2, 3 ,8, 0, 6, 7, 5, 9, 10, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::B(array<byte, 8>{0, 1, 6, 2, 4, 5, 7, 3}, array<byte, 8>{0, 0, 2, 1, 0, 0, 1, 2}, array<byte, 12>{0, 1, 6, 3, 4, 5, 10, 2, 8, 9, 7, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::U(array<byte, 8>{0, 1, 2, 3, 7, 4, 5, 6}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::D(array<byte, 8>{1, 2, 3, 0, 4, 5, 6, 7}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{1, 2, 3, 0, 4, 5, 6, 7, 8, 9, 10, 11}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
Cube Cube::L(array<byte, 8>{3, 1, 2, 7, 0, 5, 6, 4}, array<byte, 8>{1, 0, 0, 2, 2, 0, 0, 1}, array<byte, 12>{0, 1, 2, 7, 3, 5, 6, 11, 8, 9, 10, 4}, array<byte, 12>{0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1});
Cube Cube::R(array<byte, 8>{0, 5, 1, 3, 4, 6, 2, 7}, array<byte, 8>{0, 2, 1, 0, 0, 1, 2, 0}, array<byte, 12>{0, 5, 2, 3, 4, 9, 1, 7, 8, 6, 10, 11}, array<byte, 12>{0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0});

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