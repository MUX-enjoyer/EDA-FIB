#include <iostream>
using namespace std;

#include <vector>

int fixed_point(vector<int>& S, int a, int l, int r) {
    if (l > r) return 0;
    if (l == r) return l==S[l]+a ? l : 0; // SI l==S[l]+a cert --> l
                                          //              fals --> 0
    int m = (l+r)/2;
    if (m>S[m]+a) return fixed_point(S, a, m+1, r);
    else return fixed_point(S, a, l, m);
}


int main() {
    int n;
    int i = 1;
    while(cin >> n) {
        cout << "Sequence #" << i << endl;
        vector<int> S(n+1);
        for (int i = 1; i <= n; ++i) cin >> S[i];
        
        int m;
        cin >> m;
        for (int i = 0; i < m; ++i) {
            int a;
            cin >> a;
            int result = fixed_point(S, a, 1, n);
            if (result) cout << "fixed point for " << a << ": " << result << endl;
            else cout << "no fixed point for " << a << endl;
        }
        ++i;
        cout << endl;
    }
}