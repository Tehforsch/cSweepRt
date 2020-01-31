#include "utils.h"
#include "mpiVars.h"

Index localToGlobal(Index local) {
    int offsetI = worldRank / numCoresPerDirection * numCellsPerDirection;
    int offsetJ = worldRank % numCoresPerDirection * numCellsPerDirection;
    Index global;
    global.i = offsetI + local.i;
    global.j = offsetJ + local.j;
    return global;
}

int getCoreByGlobalIndex(Index index) {
    if ((index.i < 0 || index.i >= GRID_SIZE) || (index.j < 0 || index.j >= GRID_SIZE))  {
        return -1;
    }
    int core;
    int xNum = index.i / (GRID_SIZE / numCoresPerDirection);
    int yNum = index.j / (GRID_SIZE / numCoresPerDirection);
    return xNum * numCoresPerDirection + yNum;
}

int getCoreById(int id) {
    Index k;
    k.i = id / GRID_SIZE;
    k.j = id % GRID_SIZE;
    return getCoreByGlobalIndex(k);
}

