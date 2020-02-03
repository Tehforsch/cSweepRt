#include <vector>
#include "grid.h"
#include "mpiVars.h"
#include "utils.h"
#include "vector2d.h"
#include "stdio.h"

Grid getCartesianGrid() {
    Grid grid;
    grid.cells = std::vector<Cell> {};
    double xMin = 0.0;
    double xMax = 1.0;
    double yMin = 0.0;
    double yMax = 1.0;
    double cellWidth  = (xMax-xMin) / GRID_SIZE;
    double cellHeight = (yMax-yMin) / GRID_SIZE;
    Cell cells[numCellsPerDirection][numCellsPerDirection];
    for (int i = 0; i < numCellsPerDirection; i++) {
        for (int j = 0; j < numCellsPerDirection; j++) {
            Cell cell;
            cells[i][j] = cell;
            Index local = Index::fromLocal(worldRank, i, j);
            cells[i][j].index = local;
        }
    }
    for (int i = 0; i < numCellsPerDirection; i++) {
        for (int j = 0; j < numCellsPerDirection; j++) {
            Index index = Index::fromLocal(worldRank, i, j);
            std::vector<Index> successors {index.rightNeighbour(), index.bottomNeighbour()};
            // std::vector<Index> successors {index.rightNeighbour()};
            std::vector<Index> predecessors {index.leftNeighbour(), index.topNeighbour()};
            // std::vector<Index> predecessors {index.leftNeighbour()};
            for (int k = 0; k < successors.size(); k++) {
                if (successors[k].isInDomain()) {
                    cells[i][j].successors.push_back(successors[k]);
                }
            }
            for (int k = 0; k < predecessors.size(); k++) {
                if (predecessors[k].isInDomain()) {
                    cells[i][j].predecessors.push_back(predecessors[k]);
                }
            }
            grid.cells.push_back(cells[i][j]);
        }
    }
    return grid;
}

