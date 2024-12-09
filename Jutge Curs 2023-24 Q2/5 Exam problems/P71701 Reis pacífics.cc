using namespace std;
#include <iostream>
#include <vector>

int n, r;
using Mchar = vector<vector<char>>;
const int N_DIRS = 4;
vector<int> dirs_x = {-1, -1, 0, 1};
vector<int> dirs_y = {0, -1, -1, -1};

bool pos_ok(int x, int y) {
    return x >= 0 and y >= 0 and x < n and y < n;
}

bool atacat(Mchar& taulell, int x, int y) {
    for (int i = 0; i < N_DIRS; ++i) {
        int new_x = dirs_x[i] + x;
        int new_y = dirs_y[i] + y;
        if (pos_ok(new_x, new_y)) {
            if (taulell[new_x][new_y] == 'K') return true;
        }
    }
    return false;
}

void imprimir_taulell(Mchar& taulell) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << taulell[i][j];
        }
        cout << endl;
    }
    cout << "----------" << endl;
}

void pos_kings(Mchar& taulell, int x, int y, int reis) {
    int new_x;
    int new_y;
    if (x < n - 1) {
        new_x = x + 1;
        new_y = y;
    } else {
        new_x = 0;
        new_y = y + 1;
    }
    if (y >= n) {
        if (reis == r) imprimir_taulell(taulell);
        return;
    }

    if (!atacat(taulell, x, y) and reis < r) {
        taulell[x][y] = 'K';
        pos_kings(taulell, new_x, new_y, reis + 1);
    }
    taulell[x][y] = '.';
    pos_kings(taulell, new_x, new_y, reis);
}

int main() {
    cin >> n >> r;
    Mchar taulell(n, vector<char>(n));
    pos_kings(taulell, 0, 0, 0);
}