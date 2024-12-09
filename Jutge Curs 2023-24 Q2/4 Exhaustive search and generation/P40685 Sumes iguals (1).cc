#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

void generate(vector<int>& vals, vector<bool>& v, int i, int s, int total, int sum_positius, int sum_negatius) {
    if (total+sum_positius<s or total+sum_negatius>s) return;
    int n = v.size();
    if (i == n) {
        if (total == s) {
            print(v, vals);
        }
    }
    else {
        if (vals[i] < 0) sum_negatius-=vals[i];
        else sum_positius-=vals[i];

        v[i] = false;
        generate(vals, v, i+1, s, total, sum_positius, sum_negatius);
        v[i] = true;
        generate(vals, v, i+1, s, total+vals[i], sum_positius, sum_negatius);
    }
}

int main() {
    int s, n, x, sum_negatius, sum_positius;
    cin >> s >> n;
    vector<int> vals(n);
    sum_negatius = sum_positius = 0;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        vals[i] = x;
        if (x < 0) sum_negatius += x;
        else sum_positius += x;
    }

    vector<bool> sel(n);
    generate(vals, sel, 0, s, 0, sum_positius, sum_negatius);
}