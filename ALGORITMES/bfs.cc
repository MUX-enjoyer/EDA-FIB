#include <limits>
#include <list>
#include <queue>
#include <vector>

using namespace std;

typedef vector<vector<int>> graph;

void bfs(const graph& G, int node_ini) {
    int n = G.size();
    queue<int> cua_pendents;
    vector<int> dist(n, -1);
    vector<int> node_procedent(n, -1);

    // Afegeixo node_ini a la cua
    cua_pendents.push(node_ini);
    distancies[node_ini] = 0;

    // Mentre cua no buida
    while (!cua_pendents.empty()) {
        int node = cua_pendents.front();
        // MIRAR SI ÉS CASELLA OBJECTIU!
        // Si ho és puc retornar, és la dist min!
        cua_pendents.pop();
        for (int node_adj : G[node]) {
            // Si node_adj no visitats
            if (dist[node_adj] != -1) {
                // Afegeixo cua
                // Li poso la dist a node_ini
                // Li poso el node del que ve
                cua_pendents.push(node_adj);
                dist[node_adj] = dist[node] + 1;
                node_procedent[node_adj] = node;
            }
        }
    }
}