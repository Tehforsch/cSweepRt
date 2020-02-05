#include <vector>
#include "grid.h"
#include "mpiVars.h"
#include "utils.h"
#include "vector2d.h"
#include "stdio.h"

Grid getCartesianGrid() {
    Grid grid;
    grid.cells = std::vector<Cell> {};
    grid.cells.reserve(numCellsPerDirection * numCellsPerDirection);
    double xMin = 0.0;
    double xMax = 1.0;
    double yMin = 0.0;
    double yMax = 1.0;
    double cellWidth  = (xMax-xMin) / GRID_SIZE;
    double cellHeight = (yMax-yMin) / GRID_SIZE;
    Cell cells[numCellsPerDirection][numCellsPerDirection];
    for (int i = 0; i < numCellsPerDirection; i++) {
        for (int j = 0; j < numCellsPerDirection; j++) {
            cells[i][j] = Cell::fromLocal(i, j);
        }
    }
    for (int i = 0; i < numCellsPerDirection; i++) {
        for (int j = 0; j < numCellsPerDirection; j++) {
            Index index = Index::fromLocal(worldRank, i, j);
            std::vector<Neighbour> neighbours {index.rightNeighbour(), index.bottomNeighbour(), index.leftNeighbour(), index.topNeighbour()};
            for (int k = 0; k < neighbours.size(); k++) {
                if (neighbours[k].index.isInDomain()) {
                    cells[i][j].neighbours.push_back(neighbours[k]);
                    // Ugly calculation
                    int delI = neighbours[k].index.globalI - index.globalI;
                    int delJ = neighbours[k].index.globalJ - index.globalJ;
                    for (int d = 0; d < NUM_DIRECTIONS; d++) {
                        float scalarProduct = delI * DIRECTIONS[d].y + delJ * DIRECTIONS[d].x;
                        if (scalarProduct > 0) {
                            cells[i][j].successors[d].push_back(neighbours[k]);
                        }
                        else if (scalarProduct < 0) {
                            cells[i][j].predecessors[d].push_back(neighbours[k]);
                        }
                    }
                }
            }
            grid.cells.push_back(cells[i][j]);
        }
    }
    return grid;
}

Cell Cell::fromLocal(int i, int j) {
    Index local = Index::fromLocal(worldRank, i, j);
    Cell cell;
    cell.index = local;
    cell.missingInfoCount = std::vector<int>{};
    for (int d = 0; d < NUM_DIRECTIONS; d++) {
        cell.missingInfoCount.push_back(0);
        cell.photonDensities[d] = 0.0;
        cell.incomingFluxes[d] = 0.0;
        cell.sourceFunction = (local.globalI == 1 && local.globalJ == 1) ? 1.0 : 0.0;
        // cell.sourceFunction = 0.0;
        cell.absorption = 0.1;
    }
    return cell;
}

