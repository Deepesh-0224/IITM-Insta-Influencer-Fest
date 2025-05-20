#include<Graph_class.h>

// Abstract base class for all queries
class GraphAlgorithm {
public:
    virtual void Query(Graph &g) = 0;
};