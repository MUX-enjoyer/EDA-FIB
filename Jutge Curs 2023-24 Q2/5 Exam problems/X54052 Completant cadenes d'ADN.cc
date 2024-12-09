#include <iostream>
using namespace std;
#include <vector>
#include <string>

// Imprimeix la cadena d'ADN
void imprimir_adn(const vector<char>& v1, const vector<char>& v2) {
    int n = v1.size();
    for (int i = 0; i < n; ++i) cout << v1[i];
    cout << endl;
    for (int i = 0; i < n; ++i) cout << v2[i];
    cout << endl << endl;
}

// Retorna si els trossos complets d'una cadena d'ADN és correcta
bool adn_ok(const vector<char>& adn1, const vector<char>& adn2) {
    int n = adn1.size();
    for (int i = 0; i < n; ++i) {
        char a1 = adn1[i];
        char a2 = adn2[i];
        if (a1 != '.' and a2 != '.') {
            if (a1 == 'A' and a2 != 'T') return false;
            else if (a1 == 'T' and a2 != 'A') return false;
            else if (a1 == 'C' and a2 != 'G') return false;
            else if (a1 == 'G' and a2 != 'C') return false;
        }
    }
    return true;
}

// Fa totes les combinacions de cadena correctes possibles
// Pre -> Paraula (in)completa adn1 i adn2; index que estem mirant
void completar_adn(vector<char> adn1, vector<char> adn2, int idx) {
    int n = adn1.size();

    // Si la cadena està completa la imprimeixo
    if (idx == n) {
        imprimir_adn(adn1, adn2);
        return;
    }

    if (adn1[idx] == '.') {
        // Si les dues paraules estan incompletes al mateix idx:
        // realitzem les 4 combinacions possibles d'adn
        // itornem a cridar la funció sense augmentar l'idx
        if (adn2[idx] == '.') {
            adn1[idx] = 'A';
            completar_adn(adn1, adn2, idx);
            adn1[idx] = 'C';
            completar_adn(adn1, adn2, idx);
            adn1[idx] = 'G';
            completar_adn(adn1, adn2, idx);
            adn1[idx] = 'T';
            completar_adn(adn1, adn2, idx);
        }
        // Si només està incompleta adn1 completo adn2
        // de la manera adhient i passo següent idx
        else {
            if (adn2[idx] == 'A') adn1[idx] = 'T';
            else if (adn2[idx] == 'T') adn1[idx] = 'A';
            else if (adn2[idx] == 'C') adn1[idx] = 'G';
            else if (adn2[idx] == 'G') adn1[idx] = 'C';
            completar_adn(adn1, adn2, idx+1);
        }
    }
    // Si només està incompleta adn2 completo adn1
    // de la manera adhient i passo següent idx
    else if (adn2[idx] == '.') {
        if (adn1[idx] == 'A') adn2[idx] = 'T';
        else if (adn1[idx] == 'T') adn2[idx] = 'A';
        else if (adn1[idx] == 'C') adn2[idx] = 'G';
        else if (adn1[idx] == 'G') adn2[idx] = 'C';
        completar_adn(adn1, adn2, idx+1);
    }
}

int main() {
    int n;
    while(cin >> n) {
        vector<char> adn1(n);
        vector<char> adn2(n);
        for (int i = 0; i < n; ++i) cin >> adn1[i];
        for (int i = 0; i < n; ++i) cin >> adn2[i];

        // Si trossos complets correctes -> Calculo combinacions possibles
        // Si una cadena (per molt que sigui incompleta) ja és errònia
        // no trobaré cap combinació possible que no ho sigui
        if (adn_ok(adn1, adn2)) completar_adn(adn1, adn2, 0);

        cout << "----------" << endl;
    }
}