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
    MPI_Barrier(MPI_COMM_WORLD);
    for (int core = 0; core < worldSize; core++) {
        if (core == worldRank) {
            for (int d = 0; d < NUM_DIRECTIONS; d++) {
                printf("%i %g %g ------------------------------\n", worldRank, DIRECTIONS[d].x, DIRECTIONS[d].y);
                for (int i = 0; i < numCellsPerDirection; i++) {
                    printf("%i ", worldRank);
                    for (int j = 0; j < numCellsPerDirection; j++) {
                        Index index = Index::fromLocal(worldRank, i, j);
                        printf("%.1f ", grid->cells[index.localId].photonDensities[d]);
                    }
                printf("\n");
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main() {
    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    initializeTypes();
    numCoresPerDirection = sqrt(worldSize);
    numCellsPerDirection = GRID_SIZE / numCoresPerDirection;
    Grid grid = getCartesianGrid();
    solve(&grid);
    MPI_Barrier(MPI_COMM_WORLD);
    printGrid(&grid);
    MPI_Finalize();
    return 0;
}

