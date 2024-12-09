#include <iostream>
#include <vector>
using namespace std;

int posicio(double x, const vector<double>& v, int esq, int dre) {
    if (esq > dre) return -1;
    int mig = (esq + dre)/2;
    if (x < v[mig]) return posicio(x, v, esq, mig-1);
    if (x > v[mig]) return posicio(x, v, mig+1, dre);
    return mig;
}

int main() {
    
}