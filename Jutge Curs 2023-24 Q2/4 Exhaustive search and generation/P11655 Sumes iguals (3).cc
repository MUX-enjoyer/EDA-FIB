#include <iostream>
#include <vector>
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

void generate(vector<int>& vals, vector<bool>& v, int i, int s, int total, int sum_rest) {
    if (total+sum_rest<s or total>s) return;
    int n = v.size();
    if (i == n) {
        if (total == s) print(v, vals);
    }
    else {
        v[i] = false;
        generate(vals, v, i+1, s, total, sum_rest-vals[i]);
        v[i] = true;
        generate(vals, v, i+1, s, total+vals[i], sum_rest-vals[i]);
    }
}

int main() {
    int s, n, sum_rest;
    cin >> s >> n;
    sum_rest = 0;
    vector<int> vals(n);
    for (int i = 0; i < n; ++i) {
        cin >> vals[i];
        sum_rest+=vals[i];
    }

    vector<bool> sel(n);
    generate(vals, sel, 0, s, 0, sum_rest);
}