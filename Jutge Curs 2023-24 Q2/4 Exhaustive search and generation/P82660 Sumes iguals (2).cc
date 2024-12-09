#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool trobat = false;

void print(const vector<bool>& v, const vector<int>& vals) {
    int n = v.size();
    bool first = true;
    cout << "{";
    for (int i = 0; i < n; ++i) {
        if (v[i]) {
            if (first) first = false;
            else cout << ",";
            cout << vals[i];
        }
    }
    cout << "}" << endl;
}

void generate(vector<int>& vals, vector<bool>& v, int i, int s, int total) {
    if (trobat) return;
    int n = v.size();
    if (i == n) {
        if (total == s) {
            print(v, vals);
            trobat = true;
            return;
        }
    }
    else {
        v[i] = true;
        generate(vals, v, i+1, s, total+vals[i]);
        v[i] = false;
        generate(vals, v, i+1, s, total);
    }
}

int main() {
    int s, n;
    cin >> s >> n;
    vector<int> vals(n);
    for (int i = 0; i < n; ++i) cin >> vals[i];

    sort(vals.rbegin(), vals.rend());

    vector<bool> sel(n);
    generate(vals, sel, 0, s, 0);

    if (!trobat) cout << "no solution" << endl;
}