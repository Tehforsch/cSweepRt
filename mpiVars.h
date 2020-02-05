#include "vector2d.h"
#include "mpi.h"

#ifndef MPIVARS
#define MPIVARS

const int GRID_SIZE = 10;
const int NUM_DIRECTIONS = 4;
const Vec DIRECTIONS [] = {
    {1.0, 0.0},
    {0.0, 1.0}, 
    {-1.0, 0.0}, 
    {0.0, -1.0}
};
// const int NUM_DIRECTIONS = 1;
// const Vec DIRECTIONS [] = {
//     {1.0, 0.0},
// };

extern int worldRank;
extern int worldSize;
extern int numCoresPerDirection;
extern int numCellsPerDirection;

void initializeTypes();

extern MPI_Datatype mpi_flux_data_type;

#endif
