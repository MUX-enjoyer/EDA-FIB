#include <iostream>
#include <vector>
using namespace std;

void combinacions(int n, int idx, vector<int>& v) {
    if (idx == n) {
        for (int i = 0; i < n - 1; ++i) {
            cout << v[i] << " ";
        }
        cout << v[n - 1] << endl;
    } else {
        v[idx] = 0;
        combinacions(n, idx + 1, v);
        v[idx] = 1;
        combinacions(n, idx + 1, v);
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> zeros_i_uns(n);
    combinacions(n, 0, zeros_i_uns);
}