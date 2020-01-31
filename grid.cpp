#include <vector>
#include "grid.h"
#include "mpiVars.h"
#include "utils.h"
#include "vector2d.h"

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
            cell.pos = newVec(i*cellWidth, j*cellHeight);
            cell.vertices = std::vector<Vec> {
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y - cellHeight / 2.0),
                newVec(cell.pos.x + cellWidth / 2.0, cell.pos.y - cellHeight / 2.0),
                newVec(cell.pos.x + cellWidth / 2.0, cell.pos.y + cellHeight / 2.0),
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y + cellHeight / 2.0)
            };
            cells[i][j] = cell;
            cells[i][j].id = grid.cells.size();
        }
    }
    for (int i = 0; i < numCellsPerDirection; i++) {
        for (int j = 0; j < numCellsPerDirection; j++) {
            Index local;
            local.i = i;
            local.j = j;
            Index global = localToGlobal(local);
            if ((global.i+1) < GRID_SIZE) {
                cells[i][j].neighbours.push_back(cells[i+1][j].id);
            }
            if ((global.j+1) < GRID_SIZE) {
                cells[i][j].neighbours.push_back(cells[i][j+1].id);
            }
            grid.cells.push_back(cells[i][j]);
        }
    }
    return grid;
}

