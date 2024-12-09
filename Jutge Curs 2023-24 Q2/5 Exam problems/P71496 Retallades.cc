#include <iostream>
using namespace std;
#include <vector>
#include <limits.h>
#include <queue>

using Graf_pesos = vector<vector<pair<int, int>>>;

// Calcula el cost mínim de camins per visitar totes les ciutats
// Pre -> Graf de n nodes
// Post -> Cost mínim
int cost_arbre_minim(const Graf_pesos& G, int n) {
    int cost_min = 0;
    vector<bool> visitat(n, false);
    
    // First  -> Pes camí
    // Second -> Ciutat(node) al que va
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    // Afegeixo node 0 perquè començi,
    // però podria afegir qualsevol
    // (mentre li posi que té pes 0)
    pq.push({0, 0});

    while(!pq.empty()) {
        int pes = pq.top().first;
        int ciutat = pq.top().second;
        pq.pop();
        if (!visitat[ciutat]) {
            cost_min += pes;
            visitat[ciutat] = true;
            // Miro camins possibles que portin a ciutats no visitades
            // encara i els afegeix-ho a la cua de prioritats 
            for (pair<int, int> cami : G[ciutat]) {
                if (!visitat[cami.second]) pq.push(cami);
            }
        }
    }
    return cost_min;
}

int main() {
    int n, m;
    while(cin >> n >> m) {
        Graf_pesos G(n, vector<pair<int, int>>());
        int x, y, c;
        int cost_total = 0;
        for (int i = 0; i < m; ++i) {
            cin >> x >> y >> c;
            G[x].push_back({c, y});
            if (x != y) G[y].push_back({c, x});
            cost_total += c;
        }
        // estalvi_maxim = cost_total - cost_minim
        cout << cost_total - cost_arbre_minim(G, n) << endl;
    }
}



