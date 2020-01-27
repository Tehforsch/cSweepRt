#include <vector>
#include <iostream>

#include "vector2d.cpp"

using namespace std;

struct Cell {
    Vec pos;
    vector<Vec> vertices;
    vector<int> neighbours;
    int id;
};

struct Grid {
    std::vector<Cell> cells;
};

void connect(Cell &cell1, Cell &cell2) {
    cell1.neighbours.push_back(cell1.id);
    cell2.neighbours.push_back(cell1.id);
}

Grid getCartesianGrid(int numCellsOneDirection) {
    Grid grid;
    grid.cells = vector<Cell> {};
    double xMin = 0.0;
    double xMax = 1.0;
    double yMin = 0.0;
    double yMax = 1.0;
    double cellWidth  = (xMax-xMin) / numCellsOneDirection;
    double cellHeight = (yMax-yMin) / numCellsOneDirection;
    Cell cells[numCellsOneDirection][numCellsOneDirection];
    for (int i = 0; i < numCellsOneDirection; i++) {
        for (int j = 0; j < numCellsOneDirection; j++) {
            Cell cell;
            cell.pos = newVec(0.0, 0.0);
            cell.vertices = vector<Vec> {
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y - cellHeight / 2.0),
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y - cellHeight / 2.0),
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y - cellHeight / 2.0),
                newVec(cell.pos.x - cellWidth / 2.0, cell.pos.y - cellHeight / 2.0)
            };
            cells[i][j] = cell;
        }
    }
    for (int i = 0; i < numCellsOneDirection; i++) {
        for (int j = 0; j < numCellsOneDirection; j++) {
            if (i+1 < numCellsOneDirection) {
                connect(cells[i][j], cells[i+1][j]);
            }
            if (j+1 < numCellsOneDirection) {
                connect(cells[i][j], cells[i][j+1]);
            }
            cells[i][j].id = grid.cells.size();
            grid.cells.push_back(cells[i][j]);
        }
    }
    return grid;
}

int main() {
    Grid grid = getCartesianGrid(10);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // printf("%i %i %i %i", i, j, grid.cells[10*i+j].id, grid.cells[10*i+j].neighbours.size());
            printf("%i", grid.cells[10*i+j].neighbours.size());
        }
        printf("\n");
    }
    return 0;
}

