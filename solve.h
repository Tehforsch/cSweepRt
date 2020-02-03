#include "grid.h"
#include <queue>

#ifndef SOLVE
#define SOLVE

void solve(Grid* grid);
void setupCounts(Grid* grid);
std::queue<Index> initializeQueue(Grid *grid);
void solveCell(std::queue<Index> *queue, Grid* grid, Index *toSolve);
void readIncomingMessages(std::queue<Index> *queue, Grid* grid);
void sendMessage(Index* succesorIndex);
void checkSuccessorSolvable(std::queue<Index> *queue, Grid* grid, Index toSolve);
bool solved(Grid *grid);

#endif
