#include <iostream>
using namespace std;
#include <vector>
#include <queue>
#include <list>

list<int> ordenacio_topo(const vector<vector<int>>& graph, int n) {
    vector<int> grau_entrada(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < graph[i].size(); ++j) {
            ++grau_entrada[graph[i][j]];
        }
    }

    priority_queue<int, vector<int>, greater<int>> nodes_grau_zero;
    for (int i = 0; i < n; ++i) {
        if (grau_entrada[i] == 0) nodes_grau_zero.push(i);
    }

    list<int> ordre;
    while(!nodes_grau_zero.empty()) {
        int node = nodes_grau_zero.top();
        ordre.push_back(node);
        nodes_grau_zero.pop();
        for (int i = 0; i < graph[node].size(); ++i) {
            int vertex = graph[node][i];
            --grau_entrada[vertex];
            if (grau_entrada[vertex] == 0) nodes_grau_zero.push(vertex);
        }
    }
    return ordre;
}

int main() {
    int n, m, x, y;
    while (cin >> n >> m) {
        vector<vector<int>> graph(n, vector<int>());
        for (int i = 0; i < m; ++i) {
            cin >> x >> y;
            graph[x].push_back(y);
        }
        list<int> ordre = ordenacio_topo(graph, n);
        list<int>::iterator it = ordre.begin();
        cout << *it;
        for (++it; it != ordre.end(); ++it) cout << " " << *it;
        cout << endl;
    }
}