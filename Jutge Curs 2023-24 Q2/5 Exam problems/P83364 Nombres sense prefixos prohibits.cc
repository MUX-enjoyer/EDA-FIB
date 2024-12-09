#include <iostream>
using namespace std;
#include <vector>

void mirar_prefixos(int pendents, int actual,const vector<int>& div) {
    if (pendents == 0) cout << actual << endl;
    else {
        for (int num = 0; num < 10; ++num) {
            int new_actual = actual*10 + num;
            bool divisible = false;
            for (int i = 0; i < div.size(); ++i) {
                if (new_actual%div[i] == 0) divisible = true;
            }
            if (!divisible) mirar_prefixos(pendents-1, new_actual, div);
        }
    }

}

int main() {
    int n, m;
    while(cin >> n >> m) {
        vector<int> div(m);
        for (int i = 0; i < m; ++i) {
            cin >> div[i];
        }
        mirar_prefixos(n, 0, div);
        cout << "----------" << endl;
    }
}