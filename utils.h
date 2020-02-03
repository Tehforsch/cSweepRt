#include <iostream>
#ifndef UTILS
#define UTILS

class Index {
    public: 
        int localI;
        int localJ;
        int globalI;
        int globalJ;
        int localId;
        int globalId;
        int i;
        int j;
        int core;
        static Index fromLocal(int core, int i, int j);
        static Index fromGlobal(int i, int j);
        Index leftNeighbour();
        Index rightNeighbour();
        Index bottomNeighbour();
        Index topNeighbour();
        bool isInDomain();
        bool isOnThisCore();
};

std::ostream& operator<<(std::ostream& output, const Index& data);

#endif
