#include <vector>
#include "vector2d.h"

#ifndef GRID
#define GRID

struct Cell {
    Vec pos;
    std::vector<Vec> vertices;
    std::vector<int> neighbours;
    int id;
};

struct Grid {
    std::vector<Cell> cells;
};

Grid getCartesianGrid();

#endif
