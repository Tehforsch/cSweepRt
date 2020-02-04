#include <mpi.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "mpiVars.h"
#include "utils.h"
#include "solve.h"
#include "grid.h"

using namespace std;

int numCoresPerDirection;
int numCellsPerDirection;
int worldRank;
int worldSize;

void printGrid(Grid *grid) {
    for (int d = 0; d < NUM_DIRECTIONS; d++) {
        for (int i = 0; i < numCellsPerDirection; i++) {
            printf("%i ", worldRank);
            for (int j = 0; j < numCellsPerDirection; j++) {
                Index index = Index::fromLocal(worldRank, i, j);
                // printf("%i", grid->cells[index.localId].numPredecessors[d]);
                printf("%i", grid->cells[index.localId].successors[d].size());
            }
            printf("\n");
            }
        }
}

int main() {
    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    numCoresPerDirection = sqrt(worldSize);
    numCellsPerDirection = GRID_SIZE / numCoresPerDirection;
    Grid grid = getCartesianGrid();
    // printGrid(&grid);
    solve(&grid);
    MPI_Finalize();
    return 0;
}

