#ifndef UTILS
#define UTILS

struct Index {
    int i;
    int j;
};


Index localToGlobal(Index local);

int getCoreByGlobalIndex(Index index);

int getCoreById(int id);

#endif
