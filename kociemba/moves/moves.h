#pragma once

#include <array>
#include <unordered_set>
#include <algorithm>
#include "../Settings.h"
#include "../Cube.h"
#include "../Symmetry/symmetry.h"
#include "../math/Combination.h"
#include "../math/Permutation.h"
#include "../io/io.h"





namespace move{


    // returns move table for twist coordinate
    std::vector<ushort> twist_movetable();



    // returns move table for flip coordinate
    std::vector<ushort> flip_movetable();



    // returns move table for slice coordinate
    std::vector<ushort> slice_movetable();



    // returns move table for corner coordinate
    std::vector<ushort> corner_movetable();

    // returns move table for corner symmetric coordinate
    std::vector<ushort> corner_sym_movetable();

    // returns move table for corner symmetry class coordinate
    std::vector<ushort> corner_cls_movetable();



    // returns move table for corner coordinate
    std::vector<uint> layers_movetable();

    // initializes vectors to convert between compressed and non-compressed layer coordinates
    void layer_conversion(std::vector<uint>& even, std::vector<uint>& odd, std::vector<uint>& layer);

    // initializes move tables for compressed corner coordinate
    void layers_compressed_movetable(std::vector<uint>& evenmt, std::vector<uint>& oddmt);

}
