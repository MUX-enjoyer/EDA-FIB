#include <functional>
#include <queue>
#include <vector>

using namespace std;

typedef pair<int, int> P;
typedef vector<vector<P>> graph;

int mst(const graph& G) {
    int n = G.size();
    vector<bool> visitat(n, false);
    priority_queue<P, vector<P>, greater<P>> cua_pendents;
    int n_visitats = 1;
    int cost_total = 0;

    // Afegeixo els nodes adjacents del node inicial a la cua
    visitat[0] = true;
    for (P node_adj : G[0]) {
        cua_pendents.push(node_adj);
    }

    // Mentre no hagi visitat tots els nodes
    while (n_visitats < n) {
        int node = cua_pendents.top().second;
        cua_pendents.pop();

        // Si no visitat
        if (!visitat[node]) {
            // Visitat
            // Augment cost_total i n_visitats
            visitat[node] = true;
            cost_total += cua_pendents.top().first;
            ++n_visitats;
            // Afageixo cua adjacents
            for (P node_adj : G[node]) {
                cua_pendents.push(node_adj);
            }
        }
    }

    return cost_total;
}