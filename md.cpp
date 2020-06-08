#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <ctime>

using namespace std;

int main() {
    int n = 500;
    int m = 200;
    srand((unsigned)time(NULL));

    cout << n << ' ' << m << endl;

    for (int i = 0; i < m; ++i) {
        int u = rand() % n;
        int v = rand() % n;
        cout << u << ' ' << v << endl;
    }

    vector<int> seq = vector<int>(n, 0);
    for (int i = 0; i < n; ++i) {
        seq[i] = i;
    }
    random_shuffle(seq.begin(), seq.end());
    for (int i = 0; i < n; ++i) {
        cout << seq[i] << ' ';
    }
    cout << endl;
    return 0;
}