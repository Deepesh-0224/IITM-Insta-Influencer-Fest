#include<GraphAlgortihm.h>

// Derived class to check if a cycle exists
class IsCycle : public GraphAlgorithm {
public:
    void Query(Graph &g) override;
};

void IsCycle::Query(Graph &g) {
    cout << (g.cycleExist() ? "YES" : "NO") << endl;
}