// SIGUEU CONSCIENTS DE QUE AQUEST CODI
// HI POT ACCEDIR TOTHOM
// ANEU AMB COMPTE QUE COPIEU!!!
// Sí (també l'he fet per IA)

// Podeu utilitzar la plantilla de main i els exemples de Input/Output del final 
// per provar si us funciona el vostre codi de backtracking :


// ---------------------------------------------------------------------------------

// Problema: Caixes Fortes
// Tenim m nombres (pistes del joc) i volem agrupar-los en n = m/3 tripletes
// de manera que, a cada tripleta, un nombre sigui la suma dels altres dos.

#include <iostream>
#include <vector>
using namespace std;

// Imprimeix la solució trobada a partir de l'assignació "solucio"
// Cada tripleta correspondrà a una "caixa forta" resolta.
void print_solucio(const vector<int>& hints, const vector<int>& solucio) {
    int n = hints.size() / 3;  // Nombre de tripletes (caixes fortes)
    
    for (int t = 0; t < n; ++t) {
        cout << "Tripleta " << t << ": {";
        bool first = true;
        for (int i = 0; i < (int)hints.size(); ++i) {
            if (solucio[i] == t) {  // Aquest número ha estat assignat a la tripleta t
                if (!first) cout << ", ";
                cout << hints[i];
                first = false;
            }
        }
        cout << "}" << endl;
    }
}

/*
    Idea del backtracking (nivell de joc):

    Penseu que hem de repartir tots els nombres entre caixes fortes (tripletes).
    Ho fem caixa per caixa:

      1) TRIEM una combinació de 3 posicions lliures (3 nombres que encara no hem fet servir).
      2) COMPROVEM si poden formar una caixa vàlida: un nombre és la suma dels altres dos.
      3) Si és vàlid, els MARQUEM com a ocupats i passem a resoldre la següent caixa.
      4) Si més endavant quedem encallats, DESFEM aquesta elecció (backtrack) i en provem una altra.

    El backtracking explora totes les opcions d'una manera sistemàtica:
    - només continua la recerca quan l’elecció actual respecta les regles del joc;
    - si es veu que per un camí no hi ha solució, torna enrere i prova un altre camí.
*/

// Funció de backtracking per trobar una assignació vàlida de tripletes
bool backtrack(int tripleta,                    // Índex de la tripleta que estem formant (0, 1, 2, ...)
               const vector<int>& hints,        // Els números de la pista
               vector<int>& solucio,            // A quina tripleta pertany cada número
               vector<bool>& usat,              // Quins números ja hem usat
               int n) {                         // Total de tripletes (n = m/3)
    
    // CAS BASE: si hem arribat a la tripleta n, vol dir que hem omplert totes les caixes
    if (tripleta == n) {
        return true;  // Hem trobat una solució completa per al puzle
    }
    
    int m = hints.size();  // Total de números (3 * n)
    
    // PROVAR TOTES LES COMBINACIONS POSSIBLES DE 3 NÚMEROS LLIURES
    // Observació: i < j < k per no repetir la mateixa combinació en un altre ordre.
    for (int i = 0; i < m; ++i) {
        if (!usat[i]) {  // Només considerem i si encara no s'ha fet servir
            for (int j = i + 1; j < m; ++j) {
                if (!usat[j]) {  // Només considerem j si encara no s'ha fet servir
                    for (int k = j + 1; k < m; ++k) {
                        if (!usat[k]) {  // Només considerem k si encara no s'ha fet servir
                            
                            // Triem la combinació de posicions (i, j, k)
                            int a = hints[i];
                            int b = hints[j];
                            int c = hints[k];
                            
                            // VALIDAR: Comprovar si formen una tripleta vàlida
                            // Regla del joc: un dels 3 ha de ser la suma dels altres dos.
                            if (a + b == c || a + c == b || b + c == a) {
                                
                                // ESCOLLIR: Marcar aquests 3 números com a pertanyents a la tripleta actual
                                usat[i] = usat[j] = usat[k] = true;
                                solucio[i] = solucio[j] = solucio[k] = tripleta;
                                
                                // EXPLORAR: Intentar formar la següent tripleta (pas recursiu)
                                if (backtrack(tripleta + 1, hints, solucio, usat, n)) {
                                    return true;  // Èxit: propaguem el true cap amunt
                                }
                                
                                // BACKTRACK: Desfer els canvis per poder provar una altra combinació
                                usat[i] = usat[j] = usat[k] = false;
                                solucio[i] = solucio[j] = solucio[k] = -1;
                            }
                        }
                    }
                }
            }
        }
    }
    
    // No s'ha trobat cap manera de completar totes les caixes a partir de la tripleta actual
    return false;
}

int main() {
    int m;  // Nombre total de números (ha de ser múltiple de 3)
    cin >> m;
    
    vector<int> hints(m);
    for (int i = 0; i < m; ++i) {
        cin >> hints[i];  // Llegim els números de la pista del puzle
    }
    
    // Calcular nombre de tripletes (caixes)
    int n = m / 3;
    
    // Inicialitzar estructures de control pel backtracking
    vector<int> solucio(m, -1);      // solucio[i] = índex de la tripleta del número i (-1 = no assignat)
    vector<bool> usat(m, false);     // usat[i] = true si el número i ja s'ha col·locat en alguna tripleta
    
    // Executar backtracking a partir de la primera tripleta (0)
    if (backtrack(0, hints, solucio, usat, n)) {
        cout << "Solució trobada:" << endl;
        print_solucio(hints, solucio);
    } else {
        cout << "No hi ha solució" << endl;
    }
}

/*
    EXEMPLES D'ENTRADA / SORTIDA

    Exemple 1 (amb solució petita)

    Entrada
    6
    1 2 3 3 5 8

    Una possible sortida
    Solució trobada:
    Tripleta 0: {1, 2, 3}
    Tripleta 1: {3, 5, 8}

    (Pot aparèixer en un altre ordre de tripletes o elements, però les tripletes han de ser vàlides.)


    Exemple 2 (sense solució trivial)

    Entrada
    3
    1 1 1

    Sortida
    No hi ha solució


    Exemple 3 (instància més gran amb solució, nivell “joc”)

    Entrada
    9
    2 4 6 5 7 12 3 10 13

    Una possible sortida
    Solució trobada:
    Tripleta 0: {2, 4, 6}
    Tripleta 1: {5, 7, 12}
    Tripleta 2: {3, 10, 13}


    Exemple 4 (instància més gran sense solució, amb “trampes”)

    Entrada
    9
    4 5 13 10 3 4 10 8 9

    Sortida
    No hi ha solució

    Exemple 5 (12 nombres, amb solució)

    Entrada
    12
    2 3 4 5 6 7 8 9 10 11 13 16

    Una possible sortida
    Solució trobada:
    Tripleta 0: {2, 5, 7}
    Tripleta 1: {3, 8, 11}
    Tripleta 2: {4, 9, 13}
    Tripleta 3: {6, 10, 16}

    Comentari:
    - Cada tripleta compleix la regla:
        2 + 5 = 7
        3 + 8 = 11
        4 + 9 = 13
        6 + 10 = 16
    - Hi ha moltes altres combinacions candidates (per exemple, 2+3=5, 4+6=10, 5+6=11...),
      de manera que el backtracking ha de provar i desfer opcions fins a trobar
      una partició completa dels 12 nombres.


    Exemple 6 (12 nombres, sense solució)

    Entrada
    12
    2 4 6 5 7 12 3 10 13 30 30 30

    Sortida
    No hi ha solució

    Comentari:
    - Entre els 9 primers nombres hi ha tripletes vàlides com:
        {2, 4, 6}   (2 + 4 = 6)
        {5, 7, 12}  (5 + 7 = 12)
        {3, 10, 13} (3 + 10 = 13)
      però els 30 no poden aparèixer en cap tripleta vàlida:
        * cap parell de nombres de la llista suma 30
        * 30 + qualsevol altre nombre seria > 30 i aquest resultat no hi és
    - Per tant, no hi ha cap manera de repartir TOTS els 12 nombres en tripletes
      que compleixin la regla, i el backtracking acaba descartant totes les opcions.
*/
