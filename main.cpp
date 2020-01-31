#include <mpi.h>
#include <vector>
#include <iostream>
#include <math.h>
#include "mpiVars.h"
#include "utils.h"
#include "solve.h"
#include "grid.h"

using namespace std;

void printGrid(Grid *grid) {
    printf("%i", worldRank);
    printf("LOCAL\n");
    for (int i = 0; i < numCellsPerDirection; i++) {
        printf("%i ", worldRank);
        for (int j = 0; j < numCellsPerDirection; j++) {
            printf("%zu", grid->cells[numCellsPerDirection*i+j].neighbours.size());
        }
        printf("\n");
    }
}

int numCoresPerDirection;
int numCellsPerDirection;
int worldRank;
int worldSize;

int main() {
    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    numCoresPerDirection = sqrt(worldSize);
    numCellsPerDirection = GRID_SIZE / numCoresPerDirection;
    Grid grid = getCartesianGrid();
    printGrid(&grid);
    solve(&grid);
    MPI_Finalize();
    return 0;
}

