#include "utils.h"
#include "mpiVars.h"
#include "stdio.h"
#include "fstream"

Index Index::fromLocal(int core, int i, int j) {
    int globalI = i + (core / numCoresPerDirection) * numCellsPerDirection;
    int globalJ = j + (core % numCoresPerDirection) * numCellsPerDirection;
    return Index::fromGlobal(globalI, globalJ);
}

Index Index::fromGlobal(int i, int j) {
    Index index;
    index.globalI = i;
    index.globalJ = j;
    index.localI = i % numCellsPerDirection;
    index.localJ = j % numCellsPerDirection;
    index.localId = index.localI * numCellsPerDirection + index.localJ;
    index.globalId = index.globalI * GRID_SIZE + index.globalJ;
    if (index.isInDomain()) {
        index.core = (index.globalI / numCellsPerDirection) * numCoresPerDirection + (index.globalJ / numCellsPerDirection);
    }
    else {
        index.core = -1;
    }
    return index;
}

Index Index::fromGlobalId(int id) {
    int globalI = id / GRID_SIZE;
    int globalJ = id % GRID_SIZE;
    return Index::fromGlobal(globalI, globalJ);
}

Neighbour Index::leftNeighbour() {
    return Neighbour::fromIndex(Index::fromGlobal(this->globalI, this->globalJ-1));
}

Neighbour Index::rightNeighbour() {
    return Neighbour::fromIndex(Index::fromGlobal(this->globalI, this->globalJ+1));
}

Neighbour Index::bottomNeighbour() {
    return Neighbour::fromIndex(Index::fromGlobal(this->globalI+1, this->globalJ));
}

Neighbour Index::topNeighbour() {
    return Neighbour::fromIndex(Index::fromGlobal(this->globalI-1, this->globalJ));
}

Neighbour Neighbour::fromIndex(Index index) {
    return Neighbour{index, 0.0};
};

bool Index::isInDomain() {
    return (this->globalI >= 0 && this->globalJ >= 0 && this->globalI < GRID_SIZE && this->globalJ < GRID_SIZE);
}

bool Index::isOnThisCore() {
    return this->core == worldRank;
}

std::ostream& operator<< (std::ostream &out, Index const& data) {
    out << "(" << data.localI << " " << data.localJ << ")";
    out << "(" << data.globalI << " " << data.globalJ << ")";
    out << "|" << data.localId << " " << data.globalId << "|" << " on " << data.core;
    return out;
}
