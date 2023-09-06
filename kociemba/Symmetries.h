#pragma once

#include <array>
#include "Cube.h"
#include "Settings.h"

using namespace std;


namespace sym{

    // third turn around an axis through FUR and BDL corners
    Cube thirdturn(std::array<byte, 8>{2, 6, 7, 3, 1, 5, 4, 0}, array<byte, 8>{1, 2, 1, 2, 2, 1, 2, 1}, array<byte, 12>{6, 10, 7, 2, 1, 9, 11, 3, 5, 8, 4, 0}, array<byte, 12>{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1});

    // half turn around an axis that goes through the centers of F and B faces
    Cube halfturn(array<byte, 8>{5, 4, 7, 6, 1, 0, 3, 2}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{8, 11, 10, 9, 5, 4, 7, 6, 0, 3, 2, 1}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    // quorter turn around an axis that goes through the centers of D and U faces
    Cube quarturn(array<byte, 8>{3, 0, 1, 2, 7, 4, 5, 6}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10}, array<byte, 12>{0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0});

    // reflection over the plane that goes throug the middle of the slice between L and R faces
    Cube reflect(array<byte, 8>{1, 0, 3, 2, 5, 4, 7, 6}, array<byte, 8>{3, 3, 3, 3, 3, 3, 3, 3}, array<byte, 12>{0, 3, 2, 1, 5, 4, 7, 6, 8, 11, 10, 9}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

    array<Cube, 16> symmetries;

    // fills the symmetries array
    void compute(){
        Cube a, b, c;
        for(int i = 0; i < 2; ++i){
            for(int j = 0; j < 4; ++j){
                for(int k = 0; k < 2; ++k){
                    symmetries[8*i + 2*j + k] = a * b * c;
                    c *= reflect;
                }
                b *= quarturn;
            }
            a *= halfturn;
        }
    }

    void applyHalfturn2(Cube& cube){
        Cube sym(array<byte, 8>{5, 4, 7, 6, 1, 0, 3, 2}, array<byte, 8>{0, 0, 0, 0, 0, 0, 0, 0}, array<byte, 12>{8, 11, 10, 9, 5, 4, 7, 6, 0, 3, 2, 1}, array<byte, 12>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        std::array<byte, 8> recolor_corners = {5, 4, 7, 6, 1, 0, 3, 2};
        std::array<byte, 12> recolor_edges = {8, 11, 10, 9, 5, 4, 7, 6, 0, 3, 2, 1};
        cube *= sym;
        for(int i = 0; i < 8; ++i){
            cube.corner[i] = recolor_corners[cube.corner[i]];
        }
        for(int i = 0; i < 12; ++i){
            cube.edge[i] = recolor_edges[cube.edge[i]];
        }
    }

    void applyThirdturn(Cube& cube){
        cube = thirdturn.inv() * cube * thirdturn;
    }

    void applyHalfturn(Cube& cube){
        cube = halfturn.inv() * cube * halfturn;
    }

    void applyQuarturn(Cube& cube){
        cube = quarturn.inv() * cube * quarturn;
    }

    void applyReflect(Cube& cube){
        cube = reflect.inv() * cube * reflect;
    }

}