#include <vector>
#include "vector2d.h"
#include "utils.h"
#include "mpiVars.h"

#ifndef GRID
#define GRID

struct Cell {
    public:
        std::vector<Neighbour> predecessors[NUM_DIRECTIONS];
        std::vector<Neighbour> successors[NUM_DIRECTIONS];
        std::vector<Neighbour> neighbours;
        std::vector<int> missingInfoCount;
        Index index;

        bool solved;
        double sourceFunction;
        double absorption;
        double photonDensities[NUM_DIRECTIONS];
        double incomingFluxes[NUM_DIRECTIONS];
        double absorbed;

        static Cell fromLocal(int i, int j);
};

struct Grid {
    std::vector<Cell> cells;
    int unsolvedCount;
};

Grid getCartesianGrid();

#endif
