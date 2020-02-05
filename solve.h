#include "grid.h"
#include "task.h"
#include <queue>

#ifndef SOLVE
#define SOLVE

void solve(Grid* grid);
void setupCounts(Grid* grid);
std::queue<Task> initializeQueue(Grid *grid);
void solveCell(std::queue<Task> *queue, Grid* grid, Task *toSolve);
void readIncomingMessages(std::queue<Task> *queue, Grid* grid);
void sendMessage(Task* successorTask, double flux);
void checkSuccessorSolvable(std::queue<Task> *queue, Grid* grid, Task toSolve);
bool solved(Grid *grid);

struct FluxData {
    int globalId;
    int direction;
    double flux;
};

#endif
