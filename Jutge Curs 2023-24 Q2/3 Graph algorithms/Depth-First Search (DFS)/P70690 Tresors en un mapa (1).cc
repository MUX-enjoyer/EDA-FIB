#include <iostream>
using namespace std;
#include <vector>

const vector<int> d_x = {0, 0, 1, -1};
const vector<int> d_y = {1, -1, 0, 0};

bool treasure(const vector<vector<char>>& mapa, vector<vector<bool>>& visitats, int n, int m, int i, int j) {
    if (visitats[i][j]) return false;
    if (mapa[i][j] == 't') return true;
    visitats[i][j] = true;
    if (mapa[i][j]=='X') return false;

    for (int dir = 0; dir<4; ++dir) {
        int new_i = i + d_x[dir];
        int new_j = j + d_y[dir];
        if (new_i>=0 && new_j>=0 && new_i<n && new_j<m) {
            bool found = treasure(mapa, visitats, n, m, new_i, new_j);
            if (found) return true;
        }
    }
    return false;
}

int main() {
    int n, m, f, c;
    cin >> n >> m;
    vector<vector<char>> mapa(n, vector<char>(m));
    vector<vector<bool>> visitats(n, vector<bool>(m, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> mapa[i][j];
        }
    }
    cin >> f >> c;
    --f;
    --c;
    if (treasure(mapa, visitats, n, m, f, c)) cout << "yes" << endl;
    else cout << "no" << endl;
}