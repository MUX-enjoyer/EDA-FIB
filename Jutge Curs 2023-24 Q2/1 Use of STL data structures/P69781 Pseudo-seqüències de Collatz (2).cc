#include <iostream>
#include <map>

using namespace std;

int main() {
    int x, y, n;
    while (cin >> x >> y >> n) {
        map<int, int> nums;
        int i = 0;
        while (n<=1e8 && nums.find(n) == nums.end()) {
            nums.insert({n, i});
            if (n%2==0) n = n/2+x;
            else n = 3*n + y;
            ++i;
        }
        if (n > 1e8) cout << n << endl;
        else cout << i-nums[n] << endl;
    }
}