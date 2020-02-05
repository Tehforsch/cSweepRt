#include "stdio.h"
#include <iostream>
#include "mpi.h"
#include <unistd.h>

#include "solve.h"
#include "grid.h"
#include "mpiVars.h"
#include "utils.h"
#include "task.h"

void calculateFluxes(Cell* cell, int direction) {
    // double incomingFlux = 0.0;
    // for (int i = 0; i < cell->predecessors[direction].size(); i++) {
        // Neighbour predecessor = cell->predecessors[direction][i];
        // incomingFlux += predecessor.flux;
    // }
    cell->photonDensities[direction] += cell->incomingFluxes[direction];
    cell->photonDensities[direction] -= cell->photonDensities[direction] * cell->absorption;
    cell->photonDensities[direction] += cell->sourceFunction;
    for (int i = 0; i < cell->successors[direction].size(); i++) {
        Neighbour* successor = &cell->successors[direction][i];
        int delI = successor->index.globalI - cell->index.globalI;
        int delJ = successor->index.globalJ - cell->index.globalJ;
        double scalarProduct = delI * DIRECTIONS[direction].y + delJ * DIRECTIONS[direction].x;
        double faceLength = 1.0;
        successor->flux = cell->photonDensities[direction] * scalarProduct * faceLength;
    }
}

void solve(Grid *grid) {
    setupCounts(grid);
    std::queue<Task> queue = initializeQueue(grid);
    while (true) {
        while (queue.size() > 0) {
            Task toSolve = queue.front();
            queue.pop();
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
    calculateFluxes(cell, toSolve->direction);
    for (int i = 0; i < cell->successors[toSolve->direction].size(); i++) {
        Task successorTask;
        successorTask.index = cell->successors[toSolve->direction][i].index;
        successorTask.direction = toSolve->direction;
        if (successorTask.index.isOnThisCore()) {
            Cell* successorCell = &grid->cells[cell->successors[toSolve->direction][i].index.localId];
            successorCell->incomingFluxes[toSolve->direction] += cell->successors[toSolve->direction][i].flux;
            checkSuccessorSolvable(queue, grid, successorTask);
        }
        else {
            sendMessage(&successorTask, cell->successors[toSolve->direction][i].flux);
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
    FluxData buffer;
    MPI_Status status;
    MPI_Recv(&buffer, 1, mpi_flux_data_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    Task incoming;
    incoming.index = Index::fromGlobalId(buffer.globalId);
    incoming.direction = buffer.direction;
    grid->cells[incoming.index.localId].incomingFluxes[incoming.direction] += buffer.flux;

    checkSuccessorSolvable(queue, grid, incoming);
}

void sendMessage(Task* successorTask, double flux) {
    FluxData buffer = {successorTask->index.globalId, successorTask->direction, flux};
    MPI_Status status;
    MPI_Send(&buffer, 1, mpi_flux_data_type, successorTask->index.core, 0, MPI_COMM_WORLD);
    printf("%i sending (%i, %i, %g %g %g) to %i\n", worldRank, successorTask->index.globalI, successorTask->index.globalJ, DIRECTIONS[successorTask->direction].x, DIRECTIONS[successorTask->direction].y, buffer.flux, successorTask->index.core);
}

void setupCounts(Grid* grid) {
    grid->unsolvedCount = 0;
    for (int i=0; i < grid->cells.size(); i++) {
        for (int d=0; d < NUM_DIRECTIONS; d++) {
            grid->cells[i].missingInfoCount[d] = grid->cells[i].predecessors[d].size();
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
