#include <limits>
#include <list>
#include <queue>
#include <vector>

using namespace std;

typedef vector<vector<int>> graph;

void dijkstra(const graph& G, int node_ini) {
    int n = G.size();
    // priority_queue <PES, NODE> ordenat de forma creixent (menys pes primer)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cua_pendents;
    vector<int> dist(n, numeric_limits<int>::max());
    vector<int> node_procedent(n, -1);

    // Afegeixo node_ini a la cua
    cua_pendents.push(0, node_ini);
    distancies[node_ini] = 0;

    // Mentre cua no buida
    while (!cua_pendents.empty()) {
        int node = cua_pendents.front();
        // MIRAR SI ÉS CASELLA OBJECTIU!
        // Si ho és puc retornar, és la dist min!
        cua_pendents.pop();
        for (int node_adj : G[node]) {
            // Si nova_dist node_adj < actual
            int new_dist = dist[node] + node_adj.second;
            if (new_dist < dist[node_adj]) {
                // Afegeixo cua
                // Li poso la new_dist al node
                // Li poso el node del que ve
                cua_pendents.push(new_dist, node_adj.first);
                dist[node_adj] = new_dist;
                node_procedent[node_adj] = node.first;
            }
        }
    }
}
