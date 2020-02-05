#include <iostream>

#ifndef UTILS
#define UTILS

class Neighbour;

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
        static Index fromGlobalId(int id);
        bool isInDomain();
        bool isOnThisCore();
        Neighbour leftNeighbour();
        Neighbour rightNeighbour();
        Neighbour bottomNeighbour();
        Neighbour topNeighbour();

};

class Neighbour {
    public:
        Index index;
        double flux;
        static Neighbour fromIndex(Index index);
};

std::ostream& operator<<(std::ostream& output, const Index& data);

#endif
