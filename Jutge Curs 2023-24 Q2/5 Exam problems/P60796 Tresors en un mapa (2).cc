#include <iostream>
using namespace std;
#include <vector>
#include <queue>

const vector<int> d_x = {0, 0, 1, -1};
const vector<int> d_y = {1, -1, 0, 0};

int min_dist_treasure(const vector<vector<char>>& mapa, vector<vector<int>>& distancies, int n, int m, int i, int j) {
    distancies[i][j] = 0;
    
    queue<pair<int, int>> nodes_pendents;
    nodes_pendents.push({i, j});
    while(!nodes_pendents.empty()) {
        pair<int, int> node = nodes_pendents.front();
        i = node.first;
        j = node.second;
        nodes_pendents.pop();

        int dist = distancies[i][j];
        if (mapa[i][j] == 't') return dist;

        for (int dir = 0; dir<4; ++dir) {
            int new_i = i + d_x[dir];
            int new_j = j + d_y[dir];
            if (new_i>=0 && new_j>=0 && new_i<n && new_j<m && (distancies[new_i][new_j]==-1) && (mapa[new_i][new_j]!='X')) {
                nodes_pendents.push({new_i, new_j});
                distancies[new_i][new_j] = dist+1;
            }
        }
    }
    return -1;
}

int main() {
    int n, m, f, c;
    cin >> n >> m;
    vector<vector<char>> mapa(n, vector<char>(m));
    vector<vector<int>> distancies(n, vector<int>(m, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> mapa[i][j];
        }
    }
    cin >> f >> c;
    --f;
    --c;
    int dist = min_dist_treasure(mapa, distancies, n, m, f, c);
    if (dist != -1) cout << "distancia minima: " << dist << endl;
    else cout << "no es pot arribar a cap tresor" << endl;
}