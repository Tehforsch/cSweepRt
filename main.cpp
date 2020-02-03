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

int main() {
    MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    numCoresPerDirection = sqrt(worldSize);
    numCellsPerDirection = GRID_SIZE / numCoresPerDirection;
    Grid grid = getCartesianGrid();
    solve(&grid);
    MPI_Finalize();
    return 0;
}

