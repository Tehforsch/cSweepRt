#include <vector>
#include "vector2d.h"
#include "utils.h"

#ifndef GRID
#define GRID

struct Cell {
    std::vector<Index> predecessors;
    std::vector<Index> successors;
    int id;
    Index index;
    int missingInfoCount;
    bool solved;
};

struct Grid {
    std::vector<Cell> cells;
};

Grid getCartesianGrid();

#endif
