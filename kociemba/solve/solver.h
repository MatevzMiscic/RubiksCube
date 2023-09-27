#pragma one

#include <vector>
#include "../Settings.h"
#include "../Cube.h"

namespace solver{

    // initializes the solver
    bool init();

    std::vector<ubyte> solve(Cube cube);
    std::vector<ubyte> timelimit_solve(const Cube& cube, int ms);
    std::vector<ubyte> movelimit_solve(const Cube& cube, ubyte movelimit);



}