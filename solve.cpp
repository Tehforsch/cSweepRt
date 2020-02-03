#include "solve.h"
#include "grid.h"
#include "stdio.h"
#include "mpiVars.h"
#include "utils.h"
#include <iostream>
#include "mpi.h"
#include <unistd.h>

void solve(Grid *grid) {
    setupCounts(grid);
    std::queue<Index> queue = initializeQueue(grid);
    while (true) {
        while (queue.size() > 0) {
            Index toSolve = queue.front();
            queue.pop();
            // std::cout << worldRank << " solving " << toSolve << std::endl;;
            solveCell(&queue, grid, &toSolve);
        }
        if (grid->unsolvedCount == 0) {
            break;
        }
        readIncomingMessages(&queue, grid);
    }
}

void solveCell(std::queue<Index> *queue, Grid* grid, Index *toSolve) {
    Cell* cell = &grid->cells[toSolve->localId];
    cell->solved = true;
    grid->unsolvedCount -= 1;
    for (int i = 0; i < cell->successors.size(); i++) {
        Index successorIndex = cell->successors[i];
        if (successorIndex.isOnThisCore()) {
            checkSuccessorSolvable(queue, grid, successorIndex);
        }
        else {
            sendMessage(&successorIndex);
        }
    }
}

void checkSuccessorSolvable(std::queue<Index> *queue, Grid* grid, Index index) {
    Cell *successor = &grid->cells[index.localId];
    successor->missingInfoCount -= 1;
    if (successor->missingInfoCount == 0) {
        queue->push(index);
    }

}

void readIncomingMessages(std::queue<Index> *queue, Grid* grid) {
    int incomingId;
    MPI_Status status;
    MPI_Recv(&incomingId, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    Index incoming = Index::fromGlobalId(incomingId);
    checkSuccessorSolvable(queue, grid, incoming);
}

void sendMessage(Index* successorIndex) {
    int buffer = successorIndex->globalId;
    int count = 1;
    int tag = 0;
    MPI_Status status;
    MPI_Send(&buffer, count, MPI_INT, successorIndex->core, tag, MPI_COMM_WORLD);
}

void setupCounts(Grid* grid) {
    for (int i=0; i < grid->cells.size(); i++) {
        grid->cells[i].missingInfoCount = grid->cells[i].predecessors.size();
        grid->cells[i].solved = false;
    }
    grid->unsolvedCount = grid->cells.size();
}

std::queue<Index> initializeQueue(Grid *grid) {
    std::queue<Index> queue = std::queue<Index> {};
    for (int i=0; i < grid->cells.size(); i++) {
        if (grid->cells[i].missingInfoCount == 0) {
            queue.push(grid->cells[i].index);
        }
    }
    return queue;
}
