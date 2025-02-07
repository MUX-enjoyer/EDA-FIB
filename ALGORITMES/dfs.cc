// RECURSIVA

// Si no he visitat el node:
//     marco com a visitat
//     executo recursivament pels veins
void dfs_rec(const graph& G, int node, vector<boolean>& visitat,
             list<int>& llista) {
    if (!visitat[node]) {
        visitat[node] = true;
        llista.push_back(node);
        for (int node_adj : G[node]) {
            dfs_rec(G, node_adj, visitat, llista);
        }
    }
}

// Faig un DFS per cada node (per si no és conex)
// Si es vol fer només amb un node és pot fer directament
// dfs_rec(G, node, visitat, llista);
list<int> dfs_rec(const graph& G) {
    int n = G.size();
    list<int> llista;
    vector<boolean> visitat(n, false);
    for (int i = 0; i < n; ++i) {
        dfs_rec(G, i, visitat, llista);
    }
    return llista;
}

// ITERATIVA

// Per cada node afegeixo a la cua
// Mentre pila no buida
//     Miro top si no ha estat visitat
//         apunto llista
//         marco com a visitat
//         afegeixo a la cua els veins
list<int> dfs_iteratiu(const graph& G) {
    int n = G.size();
    list<int> llista;
    stack<int> pila_pendents;
    vector<bool> visitat(n, false);
    for (int i = 0; i < n; ++i) {
        // Si és vol fer només des de un node
        // es posa a la cua només l'inicial
        pila_pendents.push(i);
        while (!pila_pendents.empty()) {
            int node = pila_pendents.top();
            pila_pendents.pop();
            if (!visitat[node]) {
                visitat[node] = true;
                llista.push_back(node);
                for (int node_adj : G[node]) {
                    pila_pendents.push(node_adj);
                }
            }
        }
    }
    return llista;
}
