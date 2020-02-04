#include "stdio.h"
#include <iostream>
#include "mpi.h"
#include <unistd.h>

#include "solve.h"
#include "grid.h"
#include "mpiVars.h"
#include "utils.h"
#include "task.h"

void solve(Grid *grid) {
    setupCounts(grid);
    std::queue<Task> queue = initializeQueue(grid);
    while (true) {
        while (queue.size() > 0) {
            Task toSolve = queue.front();
            queue.pop();
            // std::cout << worldRank << " solving " << toSolve.index << " in " << DIRECTIONS[toSolve.direction].x << "," << DIRECTIONS[toSolve.direction].y << std::endl;;
            solveCell(&queue, grid, &toSolve);
        }
        if (grid->unsolvedCount == 0) {
            break;
        }
        readIncomingMessages(&queue, grid);
    }
}

void solveCell(std::queue<Task> *queue, Grid* grid, Task *toSolve) {
    Cell* cell = &grid->cells[toSolve->index.localId];
    cell->solved = true;
    grid->unsolvedCount -= 1;
    for (int i = 0; i < cell->successors[toSolve->direction].size(); i++) {
        Task successorTask;
        successorTask.index = cell->successors[toSolve->direction][i];
        successorTask.direction = toSolve->direction;
        if (successorTask.index.isOnThisCore()) {
            checkSuccessorSolvable(queue, grid, successorTask);
        }
        else {
            sendMessage(&successorTask);
        }
    }
}

void checkSuccessorSolvable(std::queue<Task> *queue, Grid* grid, Task task) {
    Cell *successor = &grid->cells[task.index.localId];
    successor->missingInfoCount[task.direction] -= 1;
    if (successor->missingInfoCount[task.direction] == 0) {
        queue->push(task);
    }
}

void readIncomingMessages(std::queue<Task> *queue, Grid* grid) {
    int buffer[2];
    MPI_Status status;
    MPI_Recv(&buffer, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    Task incoming;
    incoming.index = Index::fromGlobalId(buffer[0]);
    incoming.direction = buffer[1];
    checkSuccessorSolvable(queue, grid, incoming);
}

void sendMessage(Task* successorTask) {
    int buffer[] = {successorTask->index.globalId, successorTask->direction};
    int count = 2;
    MPI_Status status;
    MPI_Send(&buffer, count, MPI_INT, successorTask->index.core, 0, MPI_COMM_WORLD);
    printf("%i sending (%i, %i, %g %g) to %i\n", worldRank, successorTask->index.globalI, successorTask->index.globalJ, DIRECTIONS[successorTask->direction].x, DIRECTIONS[successorTask->direction].y, successorTask->index.core);
}

void setupCounts(Grid* grid) {
    grid->unsolvedCount = 0;
    for (int i=0; i < grid->cells.size(); i++) {
        for (int d=0; d < NUM_DIRECTIONS; d++) {
            grid->cells[i].missingInfoCount[d] = grid->cells[i].numPredecessors[d];
            grid->cells[i].solved = false;
            grid->unsolvedCount++;
        }
    }
}

std::queue<Task> initializeQueue(Grid *grid) {
    std::queue<Task> queue = std::queue<Task> {};
    for (int i=0; i < grid->cells.size(); i++) {
        for (int d=0; d < NUM_DIRECTIONS; d++) {
            if (grid->cells[i].missingInfoCount[d] == 0) {
                Task task;
                task.index = grid->cells[i].index;
                task.direction = d;
                queue.push(task);
            }
        }
    }
    return queue;
}
