#include <list>
#include <stack>
#include <vector>

using namespace std;

typedef vector<vector<int>> graph;

list<int> ordenacio_topologica(const graph& G) {
    int n = G.size();
    vector<int> grau_entrant(n, 0);
    stack<int> cua_pendents;
    list<int> ordre;

    // Inicialitzo els graus entrants
    for (int i = 0; i < n; ++i) {
        for (int node_adj : G[i]) {
            ++grau_entrant[node_adj];
        }
    }

    // Afegeixo els nodes sense graus entrants a la cua
    for (int i = 0; i < n; ++i) {
        if (grau_entrant[i] == 0) {
            cua_pendents.push(i);
        }
    }

    // Mentre cua no buida
    while (!cua_pendents.empty()) {
        int node = cua_pendents.top();
        cua_pendents.pop();
        ordre.push_back(node);

        for (int node_adj : G[node]) {
            // Redueixo el grau entrant dels nodes_adj
            // i si grau entrant és 0 l'afegeixo a la cua
            if (--grau_entrant[node_adj] == 0) {
                cua_pendents.push(node_adj);
            }
        }
    }
    return ordre;
}