#pragma once

#include <array>
#include "../Settings.h"
#include "../Cube.h"

namespace cnv{
    bool to_cube(std::array<ubyte, 54>& colors, Cube& cube);
}