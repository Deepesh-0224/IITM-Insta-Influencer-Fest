#include<MaxHypeScore.h>

// Main function
int main() {
    int noe, nod;  // number of entities (nodes), number of dependencies (edges)
    cin >> noe >> nod;
    vector<int> hypescores(noe);
    vector<pair<int, int>> dependencies;

    for (int i = 0; i < noe; i++) cin >> hypescores[i];

    for (int i = 0; i < nod; i++) {
        int a, b;
        cin >> a >> b;
        dependencies.push_back({a, b});
    }

    Graph g(noe, hypescores, dependencies);

    int noq, qt;  // number of queries and query type
    cin >> noq;

    while (noq--) {
        cin >> qt;
        GraphAlgorithm *ga;
        if (qt == 1) {
            ga = new IsCycle();
        } else if (qt == 2) {
            ga = new IndepComponent();
        } else if (qt == 3) {
            ga = new ValidOrder();
        } else if (qt == 4) {
            ga = new MaxHype();
        }
        ga->Query(g);
        delete ga;  // avoid memory leaks
    }
}