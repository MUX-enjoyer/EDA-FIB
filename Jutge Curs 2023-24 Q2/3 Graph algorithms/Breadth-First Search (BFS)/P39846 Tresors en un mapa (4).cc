#include <iostream>
using namespace std;
#include <queue>
#include <vector>

int n, m;
using matriu_char = vector<vector<char>>;
using matriu_int = vector<vector<int>>;
vector<int> dirs_x = {1, -1, 0, 0};
vector<int> dirs_y = {0, 0, 1, -1};

bool dins_matriu(int x, int y) {
    return x > 0 and x < n and y > 0 and y < m;
}

int bfs(const vector<vector<char>>& mapa, int x_ini, int y_ini) {
    queue<pair<int, int>> cua;
    cua.push({x_ini, y_ini});
    matriu_int mapa_dist(n, vector<int>(m, -1));
    mapa_dist[x_ini][y_ini] = 0;
    int dist_max = -1;

    while (!cua.empty()) {
        int x = cua.front().first;
        int y = cua.front().second;
        cua.pop();
        if (mapa[x][y] == 't' and mapa_dist[x][y] > dist_max) dist_max = mapa_dist[x][y];

        for (int i = 0; i < 4; ++i) {
            int new_x = x + dirs_x[i];
            int new_y = y + dirs_y[i];
            if (dins_matriu(new_x, new_y) and mapa[new_x][new_y] != 'X' and mapa_dist[new_x][new_y] == -1) {
                mapa_dist[new_x][new_y] = mapa_dist[x][y] + 1;
                cua.push({new_x, new_y});
            }
        }
    }

    return dist_max;
}

int main() {
    int f, c, dist_max;
    cin >> n >> m;
    ++n;
    ++m;
    matriu_char mapa(n, vector<char>(m));
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            cin >> mapa[i][j];
        }
    }
    cin >> f >> c;
    dist_max = bfs(mapa, f, c);
    if (dist_max == -1)
        cout << "no es pot arribar a cap tresor" << endl;
    else
        cout << "distancia maxima: " << dist_max << endl;
}