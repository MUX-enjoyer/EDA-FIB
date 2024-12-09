#include <iostream>
#include <string>
#include <map>

using namespace std;

int main() {
    string ins, p;
    map<string, int> bossa;
    while (cin >> ins) {
        if (ins == "store") {
            cin >> p;
            ++bossa[p];
        }
        else if (ins == "delete") {
            cin >> p;
            map<string, int>::iterator it = bossa.find(p);
            if (it != bossa.end()) {
                if (it->second > 1) --bossa[p];
                else bossa.erase(p);
            }    
        }
        else if (ins == "minimum?") {
            if (bossa.empty()) cout << "indefinite minimum" << endl;
            else cout << "minimum: " << bossa.begin()->first << ", " << bossa.begin()->second << " time(s)" << endl;
        }
        else if (ins == "maximum?") {
            if (bossa.empty()) cout << "indefinite maximum" << endl;
            else {
                auto it = bossa.end();
                --it;
                cout << "maximum: " << it->first << ", " << it->second << " time(s)" << endl;
            }
        }
    }
}