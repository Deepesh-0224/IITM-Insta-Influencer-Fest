#include<ValidOrder.h>

// Derived class to find the maximum hype path in the condensed graph
class MaxHype : public GraphAlgorithm {
public:
    void Query(Graph &g) override;
};

void MaxHype::Query(Graph &g) {
    vector<vector<int>> sccs;
    vector<int> node_to_scc, scc_hype;
    g.findscc(sccs, node_to_scc, scc_hype);
    map<int, vector<int>> condensed_adj;
    g.buildscc(node_to_scc, condensed_adj);
    cout << g.maxHypeScore(condensed_adj, scc_hype) << endl;
}
