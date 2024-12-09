#include <iostream>
#include <vector>
using namespace std;

int n, u;

void combinacions(int n, int idx, vector<int>& v, int k) {
    if (idx == n) {
        for (int i = 0; i < n - 1; ++i) {
            cout << v[i] << " ";
        }
        cout << v[n - 1] << endl;
    } else {
        if (idx + 1 - k <= n - u) {
            v[idx] = 0;
            combinacions(n, idx + 1, v, k);
        }
        if (k + 1 <= u) {
            v[idx] = 1;
            combinacions(n, idx + 1, v, k + 1);
        }
    }
}

int main() {
    cin >> n >> u;
    vector<int> zeros_i_uns(n);
    combinacions(n, 0, zeros_i_uns, 0);
}