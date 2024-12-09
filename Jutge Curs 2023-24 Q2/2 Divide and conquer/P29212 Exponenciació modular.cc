#include <iostream>
using namespace std;

int elevar(int n, int k, int m) {
    if (k == 0) return 1;
    if (k == 1) return n%m;
    
    if (k%2 == 1) return (((elevar(n, k/2, m)*elevar(n, k/2, m))%m)*n)%m;
    else return (elevar(n, k/2, m)*elevar(n, k/2, m))%m;
}

int main() {
    int n, k, m;
    while(cin >> n >> k >> m) {
        cout << elevar(n, k, m) << endl;
    }
}