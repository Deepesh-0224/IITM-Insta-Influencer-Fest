#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

// Graph class to represent the graph and implement all required operations
class Graph {
private:
    int vertices;  // number of nodes
    vector<int> hscores;  // hype scores for each node
    vector<pair<int, int>> deps;  // list of directed edges
    map<int, vector<int>> adjlist, nadjlist;  // adjacency list and reverse graph (nadjlist)
    vector<bool> visited;  // visited array for DFS
    vector<int> order, node_to_scc;  // order of completion in DFS and node to SCC mapping
    map<int, int> indegree;  // indegree for each node (used in topological sort)
    int mscc;  // size of the largest SCC

public:
    vector<int> top_order;  // stores topological sort result

    // Constructor
    Graph(int ver, vector<int> hype_scores, vector<pair<int, int>> ds);
    
    void createadjlist();  // build the graph
    void topological_sort();  // performs topological sorting
    bool cycleExist();  // detects if there is a cycle
    bool dfs_cycle(int s, vector<bool> &vis, vector<bool> &rec);  // helper for cycle detection
    void tgraph();  // build the transpose graph
    void dfs(int v);  // DFS traversal (normal)
    void dfss(int v, vector<int> &comp);  // DFS on transposed graph
    void findscc(vector<vector<int>> &sccs, vector<int> &node_to_scc, vector<int> &scc_hype);  // Kosaraju's to find SCCs
    void buildscc(const vector<int> &node_to_scc, map<int, vector<int>> &condensed_adj);  // build condensed graph from SCCs
    int maxHypeScore(map<int, vector<int>> &condensed_adj, vector<int> &scc_hype);  // find max hype score path in DAG
    void print();  // print the size of the largest SCC
};

// Constructor
Graph::Graph(int ver, vector<int> hype_scores, vector<pair<int, int>> ds) {
    vertices = ver;
    hscores = hype_scores;
    deps = ds;
    mscc = 0;
    visited.resize(vertices + 1, false);
    createadjlist();  // create normal graph
    tgraph();  // create transpose of the graph
}

// Create adjacency list and calculate indegrees
void Graph::createadjlist() {
    for (auto &p : deps) {
        adjlist[p.first].push_back(p.second);
        indegree[p.second]++;
    }
    for (int i = 1; i <= vertices; i++) {
        if (indegree.find(i) == indegree.end()) {
            indegree[i] = 0;
        }
    }
}

// Perform topological sort using Kahn's algorithm (min-heap based)
void Graph::topological_sort() {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (auto &p : indegree) {
        if (p.second == 0) pq.push(p.first);
    }
    while (!pq.empty()) {
        int v = pq.top(); pq.pop();
        top_order.push_back(v);
        for (int neigh : adjlist[v]) {
            if (--indegree[neigh] == 0) pq.push(neigh);
        }
    }
}

// Check for cycles using DFS
bool Graph::cycleExist() {
    vector<bool> vis(vertices + 1, false);
    vector<bool> rec(vertices + 1, false);
    for (int i = 1; i <= vertices; i++) {
        if (!vis[i] && dfs_cycle(i, vis, rec)) {
            return true;
        }
    }
    return false;
}

// DFS helper for cycle detection
bool Graph::dfs_cycle(int s, vector<bool> &vis, vector<bool> &rec) {
    vis[s] = true;
    rec[s] = true;
    for (int neigh : adjlist[s]) {
        if (!vis[neigh] && dfs_cycle(neigh, vis, rec)) return true;
        else if (rec[neigh]) return true;
    }
    rec[s] = false;
    return false;
}

// Transpose the graph
void Graph::tgraph() {
    for (auto &p : adjlist) {
        int u = p.first;
        for (int v : p.second) {
            nadjlist[v].push_back(u);
        }
    }
}

// Standard DFS
void Graph::dfs(int v) {
    visited[v] = true;
    for (int neigh : adjlist[v]) {
        if (!visited[neigh]) dfs(neigh);
    }
    order.push_back(v);  // record finish order
}

// DFS on the transposed graph
void Graph::dfss(int v, vector<int> &comp) {
    visited[v] = true;
    comp.push_back(v);
    for (int neigh : nadjlist[v]) {
        if (!visited[neigh]) dfss(neigh, comp);
    }
}

// Find strongly connected components using Kosaraju’s algorithm
void Graph::findscc(vector<vector<int>> &sccs, vector<int> &node_to_scc, vector<int> &scc_hype) {
    fill(visited.begin(), visited.end(), false);
    for (int i = 1; i <= vertices; i++) {
        if (!visited[i]) dfs(i);
    }
    fill(visited.begin(), visited.end(), false);
    node_to_scc.resize(vertices + 1, -1);
    for (int i = order.size() - 1; i >= 0; i--) {
        if (!visited[order[i]]) {
            vector<int> comp;
            dfss(order[i], comp);
            mscc = max(mscc, int(comp.size()));  // track largest SCC
            int idx = sccs.size();
            sccs.push_back(comp);
            int hype = 0;
            for (int v : comp) {
                node_to_scc[v] = idx;
                hype += hscores[v - 1];
            }
            scc_hype.push_back(hype);
        }
    }
}

// Build the condensed graph (SCC DAG)
void Graph::buildscc(const vector<int> &node_to_scc, map<int, vector<int>> &condensed_adj) {
    for (int i = 1; i <= vertices; i++) {
        for (int v : adjlist[i]) {
            if (node_to_scc[i] != node_to_scc[v]) {
                condensed_adj[node_to_scc[i]].push_back(node_to_scc[v]);
            }
        }
    }
}

// Calculate max hype score by performing DP on DAG
int Graph::maxHypeScore(map<int, vector<int>> &condensed_adj, vector<int> &scc_hype) {
    int nscc = scc_hype.size();
    vector<int> indeg(nscc, 0);
    for (auto &p : condensed_adj) {
        for (int v : p.second) {
            indeg[v]++;
        }
    }

    queue<int> q;
    vector<int> dp(scc_hype.begin(), scc_hype.end());  // initialize with individual scc hype

    for (int i = 0; i < nscc; i++) {
        if (indeg[i] == 0) q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : condensed_adj[u]) {
            dp[v] = max(dp[v], dp[u] + scc_hype[v]);
            if (--indeg[v] == 0) q.push(v);
        }
    }

    return *max_element(dp.begin(), dp.end());
}

// Print size of the largest SCC
void Graph::print() {
    cout << mscc << endl;
}
