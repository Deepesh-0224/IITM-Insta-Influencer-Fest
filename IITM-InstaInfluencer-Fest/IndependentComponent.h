#include<Iscycle.h>

// Derived class to count strongly connected components
class IndepComponent : public GraphAlgorithm {
public:
    void Query(Graph &g) override;
};

void IndepComponent::Query(Graph &g) {
    vector<vector<int>> sccs;
    vector<int> node_to_scc, scc_hype;
    g.findscc(sccs, node_to_scc, scc_hype);
    cout << sccs.size() << " ";  // number of SCCs
    g.print();  // largest SCC size
}