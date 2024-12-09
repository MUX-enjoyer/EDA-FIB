#include <iostream>
#include <queue>
using namespace std;

int main() {
	cout.setf(ios::fixed);
	cout.precision(4);
	
	string operacion;
	priority_queue<int, vector<int>, greater<int>> pq;
	double mitjana = 0.0;
	int max;
	while (cin >> operacion) {
		int x;
		if(operacion == "number") {
			cin >> x;
			if(pq.empty()) max = x;
			else if (x > max) max = x;
			mitjana += x;
			pq.push(x);
		}
		else if(operacion == "delete" and !pq.empty()) {
			mitjana -= pq.top();
			pq.pop();
		}
		
		if(!pq.empty()) {
			cout << "minimum: " << pq.top() << ", maximum: " << max << 
			", average: " << mitjana/pq.size() << endl; 
		}
		else cout << "no elements" << endl;
	}
}