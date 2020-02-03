#include "vector2d.h"

#ifndef MPIVARS
#define MPIVARS

const int GRID_SIZE = 200;
const int NUM_DIRECTIONS = 4;
const Vec DIRECTIONS [] = {
    {1.0, 0.0},
    {0.0, 1.0}, 
    {-1.0, 0.0}, 
    {0.0, -1.0}
};

extern int worldRank;
extern int worldSize;
extern int numCoresPerDirection;
extern int numCellsPerDirection;

#endif
