#include <iostream>
#include <string>
#include <map>

using namespace std;

int main() {
    string nom, accio;
    int x;
    map <string, int> jugadors;
    while (cin >> nom >> accio) {
        if (accio == "enters") {
            if (jugadors.find(nom) != jugadors.end()) cout << nom << " is already in the casino" << endl;
            else jugadors.insert({nom, 0});
        }
        else if (accio == "leaves") {
            if (jugadors.find(nom) == jugadors.end()) cout << nom << " is not in the casino" << endl;
            else {
                cout << nom << " has won " << jugadors[nom] << endl;
                jugadors.erase(nom);
            }
        }
        else if (accio == "wins") {
            cin >> x;
            if (jugadors.find(nom) == jugadors.end()) cout << nom << " is not in the casino" << endl;
            else jugadors[nom] += x;
        }
    }
    cout << "----------" << endl;
    map<string, int>::iterator it;
    for (it = jugadors.begin(); it != jugadors.end(); ++it) {
        cout << it->first << " is winning " << it->second << endl;
    }
}