#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <string>
#include <map>

using namespace std;

typedef vector<vector<int>> Graph;

list<string> ordenacio_topologica(Graph &g, map<string, int> &m, map<int, string> &m2, vector<int> &apu) {
    priority_queue<string, vector<string>, greater<string>> q;

    map<string, int>::iterator it = m.begin();

    while (it != m.end()) {
        if (apu[it->second] == 0) {
            q.push(it->first);
        }
        ++it;
    }

    list<string> l;

    while (!q.empty()) {
        string u = q.top();
        q.pop();
        l.push_back(u);
        for (int i = 0; i < g[m[u]].size(); ++i) {
            int v = g[m[u]][i];
            if (--apu[v] == 0) {
                q.push(m2[v]);
            }
        }
    }

    return l;
}

int main() {
    int m;
    string p;
    while (cin >> m) {
        map<string, int> tasks;
        map<int, string> mapa;
        Graph g(m);

        for (int i = 0; i < m; ++i) {
            cin >> p;
            tasks.insert(make_pair(p, i));
            mapa.insert(make_pair(i, p));
        }

        int n;
        cin >> n;
        string a1, a2;
        vector<int> apu(m, 0);

        for (int i = 0; i < n; ++i) {
            cin >> a1 >> a2;
            g[tasks[a1]].push_back(tasks[a2]);
            apu[tasks[a2]]++;
        }

        list<string> l = ordenacio_topologica(g, tasks, mapa, apu);

        if (l.size() != m) {
            cout << "NO VALID ORDERING" << endl;
        } else {
            for (list<string>::const_iterator it = l.begin(); it != l.end(); ++it) {
                cout << *it;
            }
            cout << endl;
        }
    }
}
