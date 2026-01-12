//Time Complexity = O(n^log3 to the base 2)
//Recurrence relation = 3T(n/2) + nlogn
//UID - 24BCS11053
#include <bits/stdc++.h>
using namespace std;

long long operations = 0;
int maxDepth = 0;

void complexRec(int n, int depth) {
    maxDepth = max(maxDepth, depth);

    if (n <= 2) {
        operations++;
        return;
    }

    int p = n;
    while (p > 0) {
        vector<int> temp(n);
        operations += n;
        for (int i = 0; i < n; i++) {
            temp[i] = i ^ p;
            operations++;
        }
        p >>= 1;
        operations++;
    }

    vector<int> small(n);
    for (int i = 0; i < n; i++) {
        small[i] = i * i;
        operations++;
    }

    reverse(small.begin(), small.end());
    operations += n;

    complexRec(n / 2, depth + 1);
    complexRec(n / 2, depth + 1);
    complexRec(n / 2, depth + 1);
}

int main() {
   
        auto start = chrono::high_resolution_clock::now();
        complexRec(100, 1);
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        cout << "n = " << 100 << endl;
        cout << "Operations = " << operations << endl;
        cout << "Recursion Depth = " << maxDepth << endl;
        cout << "Time (ms) = " << duration.count() << endl;
}