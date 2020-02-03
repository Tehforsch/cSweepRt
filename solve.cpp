#include "solve.h"
#include "grid.h"
#include "stdio.h"
#include "mpiVars.h"
#include "utils.h"
#include <iostream>
#include "mpi.h"

void solve(Grid *grid) {
    setupCounts(grid);
    std::queue<Index> queue = initializeQueue(grid);
    while (queue.size() > 0) {
        Index toSolve = queue.front();
        queue.pop();
        std::cout << worldRank << " solving " << toSolve << std::endl;;
        solveCell(&queue, grid, &toSolve);
        readIncomingMessages(&queue, grid);
    }
}

void solveCell(std::queue<Index> *queue, Grid* grid, Index *toSolve) {
    Cell cell = grid->cells[toSolve->localId];
    cell.solved = true;
    for (int i = 0; i < cell.successors.size(); i++) {
        Index successorIndex = cell.successors[i];
        if (successorIndex.isOnThisCore()) {
            Cell *successor = &grid->cells[successorIndex.localId];
            successor->missingInfoCount -= 1;
            if (successor->missingInfoCount == 0) {
                queue->push(successorIndex);
            }
        }
        else {
            std::cout << worldRank << " nonlocl " << successorIndex << std::endl;
            sendMessage(&successorIndex);
        }
    }
}

void readIncomingMessages(std::queue<Index> *queue, Grid* grid) {
    // MPI_IRecv
}

void sendMessage(Index* successorIndex) {
    int buffer = 17;
    int count = 1;
    int tag = 1234;
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Isend(&buffer, count, MPI_INT, successorIndex->core, tag, MPI_COMM_WORLD, &request);
}

void setupCounts(Grid* grid) {
    for (int i=0; i < grid->cells.size(); i++) {
        grid->cells[i].missingInfoCount = grid->cells[i].predecessors.size();
        grid->cells[i].solved = false;
    }
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
