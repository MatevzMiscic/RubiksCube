#pragma once

#include <array>
#include <vector>
#include "../Cube.h"
#include "../Settings.h"



namespace sym{

    // third turn around an axis through FUR and BDL corners
    extern Cube thirdturn;

    // half turn around an axis that goes through the centers of F and B faces
    extern Cube halfturn;

    // quorter turn around an axis that goes through the centers of D and U faces
    extern Cube quarturn;

    // reflection over the plane that goes through the middle of the slice between L and R faces
    extern Cube reflect;

    extern std::array<Cube, 16> symmetries;
    extern std::array<Cube, 16> symmetries_inv;

    // fills the symmetries and symmetries_inv arrays
    void compute();

    // returns the symmetry table for corner coordinate
    std::vector<ushort> corner_st();

    // parameter st must be symmetry table for corner coordinate
    // returns vector of representatives for symmetry classes of corner coordinate
    std::vector<ushort> corner_rt(std::vector<ushort>& st);

    // initializes vectors to convert between raw and symmetric corner cooradinates
    void corner_conversion(std::vector<ushort>& sym_to_raw, std::vector<ushort>& raw_to_sym);

    // returns the inverse symmetry table for layer coordinate
    std::vector<uint> layer_ist();

    // initializes inverse symmetry tables for compressed layer coordinate
    void layer_compressed_ist(std::vector<uint>& even_ist, std::vector<uint>& odd_ist);

}