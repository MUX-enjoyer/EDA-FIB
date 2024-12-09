#include <iostream>
#include <string>
#include <set>

using namespace std;

int main() {
    set<string> s;
    auto mediana = s.end();

    string w;
    cin >> w;
    while (w != "END") {
        auto it = s.insert(w).first;
        if (mediana == s.end()) mediana = it;
        else if (s.size()%2 == 0 and w < *mediana) --mediana;
        else if (s.size()%2 != 0 and w > *mediana) ++mediana;
        cout << *mediana << endl;
        cin >> w;
    }
}