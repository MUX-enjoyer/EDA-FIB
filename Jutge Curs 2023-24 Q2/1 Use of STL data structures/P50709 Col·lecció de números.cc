#include <iostream>
#include <queue>

using namespace std;

int main() {
    priority_queue<int> pq;
    char c;
    int x;
    while(cin >> c) {
        if (pq.empty() && c != 'S') {
            if (c == 'I' || c == 'D') cin >> x;
            cout << "error!" << endl;
        }
        else if (c == 'S') {
            cin >> x;
            pq.push(x);
        }
        else if (c == 'A') {
            cout << pq.top() << endl;
        }   
        else if (c == 'R') {
            pq.pop();
        }
        else if (c == 'I') {
            cin >> x;
            x += pq.top();
            pq.pop();
            pq.push(x);
        }
        else if (c == 'D') {
            cin >> x;
            x = pq.top() - x;
            pq.pop();
            pq.push(x);
        }
    }
}