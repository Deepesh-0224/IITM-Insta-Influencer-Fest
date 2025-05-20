#include<IndependentComponent.h>

// Derived class to get valid topological ordering
class ValidOrder : public GraphAlgorithm {
public:
    void Query(Graph &g) override;
};

void ValidOrder::Query(Graph &g) {
    if (!g.cycleExist()) {
        g.topological_sort();
        for (int x : g.top_order) cout << x << " ";
        cout << endl;
    } else {
        cout << "NO" << endl;
    }
}