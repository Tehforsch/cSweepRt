#include <vector>
#include "vector2d.h"
#include "utils.h"
#include "mpiVars.h"

#ifndef GRID
#define GRID

struct Cell {
    public:
        std::vector<int> numPredecessors;
        std::vector<Index> successors[NUM_DIRECTIONS];
        std::vector<Index> neighbours;
        std::vector<int> missingInfoCount;
        int id;
        Index index;
        bool solved;
        static Cell fromLocal(int i, int j);
};

struct Grid {
    std::vector<Cell> cells;
    int unsolvedCount;
};

Grid getCartesianGrid();

#endif
